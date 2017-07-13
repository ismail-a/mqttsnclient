/*******************************************************************************
 * Copyright (c) 2014 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ismail Arai
 *
 * Description:
 * Work on linux UDP protocol with Paho MQTTSNGateway.
 *******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "MQTTSNPacket.h"
#include "transport.h"

void exitPublisher(void);
void usage(const char *);

int main(int argc, char** argv)
{
	char option;
	extern char *optarg;
	extern int optind;
	const char *command = argv[0];

	int rc = 0;
	int mysock;
	unsigned char buf[512];
	int buflen = sizeof(buf);
	MQTTSN_topicid topic;
	MQTTSNString topicstr;
	int len = 0;
	int retained = 0;
	char *topicname = "MyTopicName";
	char *clientID = "d:quickstart:udptest:9002f7f1ad23";
	char *host = "127.0.0.1";
	int port = 10000;
	unsigned char message[512] = "Message";
	MQTTSNPacket_connectData options = MQTTSNPacket_connectData_initializer;
	unsigned short topicid;

	while ((option = getopt(argc, argv, "h:p:t:c:m:")) != -1) {
		switch(option) {
			case 'h':
				host = argv[optind - 1];
				break;
			case 'p':
				port = atoi(optarg);
				break;
			case 't':
				topicname = argv[optind - 1];
				break;
			case 'c':
				clientID = argv[optind - 1];
				break;
			case 'm':
				strcpy((char *)message, argv[optind - 1]);
				break;
			default:
				usage(command);
		}
	}

	mysock = transport_open();
	if(mysock < 0)
		return mysock;

	printf("Sending to hostname %s port %d\n", host, port);

	options.clientID.cstring = clientID;
	len = MQTTSNSerialize_connect(buf, buflen, &options);
	rc = transport_sendPacketBuffer(host, port, buf, len);

	/* wait for connack */
	if (MQTTSNPacket_read(buf, buflen, transport_getdata) == MQTTSN_CONNACK)
	{
		int connack_rc = -1;

		if (MQTTSNDeserialize_connack(&connack_rc, buf, buflen) != 1 || connack_rc != 0)
		{
			printf("Unable to connect, return code %d\n", connack_rc);
			exitPublisher();
		}
		else 
			printf("connected rc %d\n", connack_rc);
	}
	else
	{
		printf("could not connect to gateway\n");
		exitPublisher();
	}

	/* register topic name */
	printf("Registering\n");
	int packetid = 1;
	topicstr.cstring = topicname;
	topicstr.lenstring.len = strlen(topicname);
	len = MQTTSNSerialize_register(buf, buflen, 0, packetid, &topicstr);
	rc = transport_sendPacketBuffer(host, port, buf, len);

	if (MQTTSNPacket_read(buf, buflen, transport_getdata) == MQTTSN_REGACK) 	/* wait for regack */
	{
		unsigned short submsgid;
		unsigned char returncode;

		rc = MQTTSNDeserialize_regack(&topicid, &submsgid, &returncode, buf, buflen);
		if (returncode != 0)
		{
			printf("return code %d\n", returncode);
			exitPublisher();
		}
		else
			printf("regack topic id %d\n", topicid);
	}
	else
		exitPublisher();

	printf("Publishing\n");
	topic.type = MQTTSN_TOPIC_TYPE_NORMAL;
	topic.data.id = topicid;

	int messagelen = strlen((const char *)message);
	len = MQTTSNSerialize_publish(buf, buflen, 0, 0, retained, 0, topic, message, messagelen);
	rc = transport_sendPacketBuffer(host, port, buf, len);
	printf("rc %d from send packet for publish length %d\n", rc, len);

	exitPublisher();

	return 0;
}

void usage(const char *command)
{
	fprintf(stderr, "Usage: %s [-h host ip address] [-p target port] [-t topic name] [-c client id] [-m message]\n", command);
	fprintf(stderr, "Default value:\n  host ip address: 127.0.0.1\n  target port: 10000\n  topic name: MyTopicName\n  client id: d:quickstart:udptest:9002f7f1ad23\n  message: Massage\n");

	exit(-1);
}

void exitPublisher(void)
{
	transport_close();

	exit(0);
}
