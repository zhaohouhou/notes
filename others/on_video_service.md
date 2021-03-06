# RTP / RTSP

- RTP（Real-time Transport Protocol）：实时传输协议。是用于Internet上针对多媒体数据流的一种传输协议。
RTP被定义为在一对一或一对多的传输情况下工作，其目的是提供时间信息和实现流同步。
RTP本身并不能为接顺序传送数据包提供可靠的传送机制，也不提供流量控制或拥塞控制。它依靠RTCP提供这些服务。
- RTCP(Real-time Transport Control Protocol)实时传输控制协议，提供流量控制和拥塞控制。
在RTP会话期间,各参与者周期性地传送RTCP包。RTCP包中含有已发送的数据包的数量、丢失的数据包的数量等统计资料.因此,服务器可以利用这些信息动态地改变传输速率
[ 实时传输协议RTP（Real-time Transport Protocol）是一个网络传输协议，它是由IETF的多媒体传输工作小组1996年在RFC 1889中公布的，
后在RFC3550中进行更新。RTP标准定义了两个子协议 ，RTP和RTCP. 当应用程序建立一个RTP会话时，应用程序将确定一对目的传输地址。
目的传输地址由一个网络地址和一对端口组成，有两个端口：一个给RTP包，一个给RTCP包，使得RTP/RTCP数据能够正确发送。
RTP数据发向偶数的UDP端口，而对应的控制信号RTCP数据发向相邻的奇数UDP端口（偶数的UDP端口＋1），这样就构成一个UDP端口对。RTP/RTCP被划分在**传输层**]
- RTSP：实时流协议（Real Time Streaming Protocol，RTSP）。**应用层**协议，位于RTP和RTCP之上，使用TCP或RTP完成数据传输。可以对流媒体提供诸如播放、暂停、快进等操作，它负责定义具体的控制消息、操作方法、状态码等，此外还描述了与RTP间的交互操作（RFC2326）。RTSP被用于建立的控制媒体流的传输，它为多媒体服务扮演“网络远程控制”的角色。尽管有时可以把RTSP控制信息和媒体数据流交织在一起传送，但一般情况RTSP本身并不用于转送媒体流数据。媒体数据的传送可通过RTP/RTCP等协议来完成。
（summary：RTSP发起/终结流媒体、RTP传输流媒体数据 、RTCP对RTP进行控制，同步。）

# RTSP request types

RTSP defines control sequences useful in controlling multimedia playback. While HTTP is stateless, RTSP has state; an identifier is used when needed to track concurrent sessions. Like HTTP, RTSP uses TCP to maintain an end-to-end connection and, while most RTSP control messages are sent by the client to the server, some commands travel in the other direction (i.e. from server to client).

- OPTIONS: An OPTIONS request returns the request types the server will accept. 无 session id
- DESCRIBE: A DESCRIBE request includes an RTSP URL (rtsp://...), and the type of reply data that can be handled. This reply includes the presentation description, typically in Session Description Protocol (SDP) format.
- SETUP: A SETUP request specifies how a single media stream must be transported. This must be done before a PLAY request is sent. Server 的 SETUP 请求会返回一个 session id，PLAY, PAUSE, RECORD, ANNOUNCE, TEARDOWN, GET_PARAMETER 请求都是有 session 的。
- PLAY: A PLAY request will cause one or all media streams to be played. Play requests can be stacked by sending multiple PLAY requests. A range can be specified. If no range is specified, the stream is played from the beginning and plays to the end, or, if the stream is paused, it is resumed at the point it was paused.
- PAUSE: A PAUSE request temporarily halts one or all media streams, so it can later be resumed with a PLAY request. The request contains an aggregate or media stream URL. A range parameter on a PAUSE request specifies when to pause. When the range parameter is omitted, the pause occurs immediately and indefinitely.
- RECORD
- ANNOUNCE
- TEARDOWN: A TEARDOWN request is used to terminate the session. It stops all media streams and frees all session related data on the server.
- GET_PARAMETER: （keep_alive 可以用这个。 GET_PARAMETER 请求带 session id）
- SET_PARAMETER
- REDIRECT

ref: （https://en.wikipedia.org/wiki/Real_Time_Streaming_Protocol）

# Video Encoding, Decoding, and Transcoding

**Codec = COder + DECoder**

## Video Encoding

Live video encoding is the process of compressing large, raw video and audio files so that they use less network bandwidth. 
Compression significantly reduces the bandwidth required, making it possible for real-time video streams or files to be easily 
transmitted across constrained networks such as the public internet.

When working with file-based video encoding, encoders are used to compress and reduce the size of video content
so that it can take up less storage space and be easier to transfer.

Compression algorithms (known as codecs) such as H.264/AVC or H.265/HEVC an reduce the raw content data by as much as 1,000 times.

Hardware encoders: low latency. Software encoders: don’t offer ultra low latency levels comparable to dedicated hardware encoders, not suitable for live broadcast applications.

## Video decoding

video decoding is the process of decoding or uncompressing encoded video in real-time.

## Video transcoding

Video transcoding is the process of converting an encoded stream from one format to another, or from one size to another. The source video can either be real-time or file-based. Most transcoders use a two-step process of decoding and re-encoding.

## FFmpeg

FFmpeg is the leading multimedia framework, able to decode, encode, transcode, mux, demux, stream, filter and play pretty much anything that humans and machines have created. It supports the most obscure ancient formats up to the cutting edge.

It contains libavcodec, libavutil, libavformat, libavfilter, libavdevice, libswscale and libswresample which can be used by applications. As well as ffmpeg, ffplay and ffprobe which can be used by end users for transcoding and playing.

# Container

Container 文件: 一些媒体文件可以同时携带视频和音频信息，例如.avi (Microsoft AVI files)、.mov (Apple QuickTime files)、MP4等container format。

container文件将视频和音频块交织地放在一起，典型结构为：

```
file header
  title, creator, other meta-info
  video header
    video codec FourCC
    width, height, colorspace, playback framerate
  audio header
    audio codec FourCC
    bits/sample, playback frequency, channel count
file data
  encoded audio chunk #0
  encoded video chunk #0
  encoded audio chunk #1
  encoded video chunk #1
  encoded audio chunk #2
  encoded video chunk #2
  encoded audio chunk #3
  encoded video chunk #3
   ...
```

FourCC code标志音视频具体的编码类型。

Summary：

- 创建多媒体文件：1）使用编码器（coder）编码音视频数据。2）使用muxer将音视频流封装成container 文件。
- 播放多媒体文件：1）使用demuxer拆分音视频流，即解封装。2）使用decoder解码音视频数据。

