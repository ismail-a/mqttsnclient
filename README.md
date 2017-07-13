# mqttsnclient
## 概要
Pahoで公開されている組み込み機器向けのMQTT-SN（[paho.mqtt-sn.embedded-c](https://github.com/eclipse/paho.mqtt-sn.embedded-c)）の中にあるクライアントのサンプルプログラムを動作確認しやすいようにオプション対応したものです。

## ビルド
MQTT-SNゲートウェイやクライアントのビルドに必要なプログラムは[paho.mqtt-sn.embedded-cのGitHubリポジトリ](https://github.com/eclipse/paho.mqtt-sn.embedded-c)にあるので、クローンしてください。

MQTT-SNゲートウェイはPahoプロジェクトのドキュメントを参考にして構築してください。

本プロジェクトのクライアントはPahoプロジェクトにあるMQTTSNPacketディレクトリ内にあるファイルに依存しますので、そのパスをbuildスクリプト内の"Path"部分を書き換えてビルドしてください。

```
PATH_TO_MQTTSNPacket="Path"
```

```
$ sh build
```

## 使い方
ビルドして出来上がったmqttsnclientにMQTTSNゲートウェイ, ポート, トピック名, クライアントID, メッセージをオプションで指定できます。何も指定しないとローカルホストにゲートウェイがあるものとしてテストメッセージが送信されます。

```
Usage: ./mqttsnclient [-h host ip address] [-p target port] [-t topic name] [-c client id] [-m message]
Default value:
  host ip address: 127.0.0.1
  target port: 10000
  topic name: MyTopicName
  client id: d:quickstart:udptest:9002f7f1ad23
  message: Massage
```

実行例は下記の通り。

クライアント：
```
$ ./mqttsnclient
Sending to hostname 127.0.0.1 port 10000
connected rc 0
Registering
regack topic id 1
Publishing
rc 0 from send packet for publish length 14
$
```

ゲートウェイ：
```
./MQTT-SNGateway

 ***************************************************************************
 * MQTT-SN Transparent Gateway
 * Part of Project Paho in Eclipse
 * (http://git.eclipse.org/c/paho/org.eclipse.paho.mqtt-sn.embedded-c.git/)
 *
 * Author : Tomoaki YAMAGUCHI
 * Version: 1.0.0
 ***************************************************************************

20170713 143839.324 PahoGateway-01 has been started.

 ConfigFile: ./gateway.conf
 SensorN/W:  UDP Multicast 225.1.1.1:1883 Gateway Port 10000
 Broker:     localhost : 1883, 8883
 RootCApath: (null)
 RootCAfile: (null)
 CertKey:    (null)
 PrivateKey: (null)
 
20170713 143920.891   CONNECT           <---  d:quickstart:udptest:9002f7f1ad2    04 04 01 00 0A 64 3A 71 75 69 63 6B 73 74 61 72 74 3A 75 64 70 74 65 73 74 3A 39 30 30 32 66 37 66 31 61 64 32 33
20170713 143920.892   CONNECT           --->  d:quickstart:udptest:9002f7f1ad2    10 2D 00 04 4D 51 54 54 04 02 00 0A 00 21 64 3A 71 75 69 63 6B 73 74 61 72 74 3A 75 64 70 74 65 73 74 3A 39 30 30 32 66 37 66 31 61 64 32 33
20170713 143921.363   CONNACK           <---  d:quickstart:udptest:9002f7f1ad2    20 02 00 00
20170713 143921.363   CONNACK           --->  d:quickstart:udptest:9002f7f1ad2    05 00

20170713 143921.363   REGISTER    0001  <---  d:quickstart:udptest:9002f7f1ad2    0A 00 00 00 01 4D 79 54 6F 70 69 63 4E 61 6D 65
20170713 143921.364   REGACK      0001  --->  d:quickstart:udptest:9002f7f1ad2    0B 00 01 00 01 00

20170713 143921.364   PUBLISH     0000  <---  d:quickstart:udptest:9002f7f1ad2    0C 00 00 01 00 00 4D 65 73 73 61 67 65
20170713 143921.364   PUBLISH     0000  --->  d:quickstart:udptest:9002f7f1ad2    30 14 00 0B 4D 79 54 6F 70 69 63 4E 61 6D 65 4D 65 73 73 61 67 65
 ```

## リンク
- [Eclipse Paho MQTT-SN Embedded C (GitHub)](https://github.com/eclipse/paho.mqtt-sn.embedded-c)

