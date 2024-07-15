#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTime>
#include <QIcon>
#include <QPixmap>
#include <QFileDialog>
#include <QMessageBox>
#include <QSlider>
#include <QComboBox>
#include<QFileDialog>
#include<QMessageBox>
#include<QUrl>
#include<iostream>
#include<QByteArray>
#include<QAudioOutput>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavdevice/avdevice.h>
#include <libavformat/version.h>
#include <libavutil/time.h>
#include <libavutil/mathematics.h>
#include <libavutil/imgutils.h>
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    QObject::connect(ui->pushButton, &QPushButton::pressed, [=]() {
//            on_open_button_clicked();
//            qDebug()<<"按下按钮";
//        });

}

MainWindow::~MainWindow()
{
    delete ui;
}

// 延时函数
void delay(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::on_pushButton_clicked()
{

    unsigned char* buf;
    int isVideo = -1;
    int ret;
    unsigned int i, streamIndex = 0;
    const AVCodec *pCodec;
    AVPacket *pAVpkt;
    AVCodecContext *pAVctx;
    AVFrame *pAVframe, *pAVframeRGB;
    AVFormatContext* pFormatCtx;
    struct SwsContext* pSwsCtx;

    avformat_network_init();


    QByteArray letter= filename.toUtf8();
    char* program_char = letter.data();

    // 创建AVFormatContext
    pFormatCtx = avformat_alloc_context();

    // 初始化pFormatCtx
    if (avformat_open_input(&pFormatCtx, program_char, 0, 0) != 0)
    {
        qDebug("6");
        avformat_free_context(pFormatCtx); // 释放资源
        return;
    }

    // 获取音视频流数据信息
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
    {
        avformat_close_input(&pFormatCtx);
        qDebug("5");
        return;
    }

    // 找到视频流的索引
    for (i = 0; i < pFormatCtx->nb_streams; i++)
    {
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            streamIndex = i;
            isVideo = 0;
            break;
        }
    }

    // 没有视频流就退出
    if (isVideo == -1)
    {
        avformat_close_input(&pFormatCtx);
        qDebug("4");
        return;
    }

    // 获取视频流编码
    pAVctx = avcodec_alloc_context3(NULL);

    // 查找解码器
    avcodec_parameters_to_context(pAVctx, pFormatCtx->streams[streamIndex]->codecpar);

    pCodec = avcodec_find_decoder(pAVctx->codec_id);
    if (pCodec == NULL)
    {
        avcodec_free_context(&pAVctx); // 释放资源
        avformat_close_input(&pFormatCtx);
        qDebug("3");
        return;
    }

    //音频，这段代码只解码了音频，没实现，实在不会了。。。
    isVideo = -1;
       for (uint i = 0; i < pFormatCtx->nb_streams; i++)
       {
           if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
           {
               isVideo = i;
               break;
           }
       }

       //有些没有音频流,所以这里不用返回
       if (isVideo == -1)
       {
           avformat_close_input(&pFormatCtx);

           qDebug()<< "find audio stream index error";
       }
       //else
      // {
           //获取音频流

           //AVStream *audioStream = pFormatCtx->streams[isVideo];
           AVCodecContext * audioCodecpar;// = audioStream->codecpar;
           // 获取视频流编码
               audioCodecpar = avcodec_alloc_context3(NULL);

           //获取音频流解码器,或者指定解码器.AVCodecContext *pAVctx
           avcodec_parameters_to_context(audioCodecpar, pFormatCtx->streams[streamIndex]->codecpar);
           const AVCodec * audioDecoder = avcodec_find_decoder(audioCodecpar->codec_id);//audioCodecpar与上面pAVctx相仿
           if (audioDecoder == NULL)
           {
               qDebug() << "audio codec not found";
               return;
           }

           //打开音频解码器

           if (avcodec_open2(audioCodecpar,audioDecoder,NULL)< 0)
           {
               qDebug()<< "open audio codec error";
               return;
           }

//}


    // 初始化pAVctx
    if (avcodec_open2(pAVctx, pCodec, NULL) < 0)
    {
        avcodec_free_context(&pAVctx); // 释放资源
        avformat_close_input(&pFormatCtx);
        qDebug("2");
        return;
    }

    // 初始化pAVpkt
    pAVpkt = av_packet_alloc();
    if (!pAVpkt)
    {
        avcodec_free_context(&pAVctx); // 释放资源
        avformat_close_input(&pFormatCtx);
        qDebug("1");
        return;
    }

    // 初始化数据帧空间
    pAVframe = av_frame_alloc();
    pAVframeRGB = av_frame_alloc();
    if (!pAVframe || !pAVframeRGB)
    {
        av_packet_free(&pAVpkt); // 释放资源
        avcodec_free_context(&pAVctx);
        avformat_close_input(&pFormatCtx);
        qDebug("555");
        return;
    }

    // 创建图像数据存储buf
    buf = (unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_RGB32, pAVctx->width, pAVctx->height, 1));
    av_image_fill_arrays(pAVframeRGB->data, pAVframeRGB->linesize, buf, AV_PIX_FMT_RGB32, pAVctx->width, pAVctx->height, 1);

    // 根据视频宽高重新调整窗口大小 视频宽高 pAVctx->width, pAVctx->height
    ui->video_window->setGeometry(0, 0, pAVctx->width, pAVctx->height);
    ui->video_window->setScaledContents(true);  // 设置 QLabel 内容缩放

    // 初始化pSwsCtx
    pSwsCtx = sws_getContext(pAVctx->width, pAVctx->height, pAVctx->pix_fmt, pAVctx->width, pAVctx->height, AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);

    // 循环读取视频数据
    int mVideoPlaySta = 1;
    while (true)
    {
        if (mVideoPlaySta == 1) // 正在播放
        {
            if (av_read_frame(pFormatCtx, pAVpkt) >= 0) // 读取一帧未解码的数据
            {
                // 如果是视频数据
                if (pAVpkt->stream_index == (int)streamIndex)
                {
                    // 解码一帧视频数据
                    ret = avcodec_send_packet(pAVctx, pAVpkt);
                    if (ret < 0)
                    {
                        qDebug("555");
                        av_packet_unref(pAVpkt);
                        continue;
                    }

                    ret = avcodec_receive_frame(pAVctx, pAVframe);
                    if (ret == 0)
                    {
                        sws_scale(pSwsCtx, (const unsigned char* const*)pAVframe->data, pAVframe->linesize, 0, pAVctx->height, pAVframeRGB->data, pAVframeRGB->linesize);
                        QImage img((uchar*)pAVframeRGB->data[0], pAVctx->width, pAVctx->height, QImage::Format_RGB32);
                        ui->video_window->setPixmap(QPixmap::fromImage(img));
                        //播放音频



                        delay(13); // 添加延时
                    }
                    else if (ret != AVERROR(EAGAIN) && ret != AVERROR_EOF)
                    {
                        qDebug("666");
                    }
                }
                //解码
                av_packet_unref(pAVpkt);
            }
            else
            {
                break;
            }
        }
        else
        {
            delay(300);
        }
    }

    // 释放资源
    sws_freeContext(pSwsCtx);
    av_frame_free(&pAVframeRGB);
    av_frame_free(&pAVframe);
    av_packet_free(&pAVpkt);
    avcodec_free_context(&pAVctx);
    avcodec_free_context(&audioCodecpar);
    avformat_close_input(&pFormatCtx);
    qDebug() << "play finish!";
}




void MainWindow::on_pushButton_2_clicked(){
    filename = QFileDialog::getOpenFileName(this, "title","D://","");
    qDebug()<<filename;
    ui->lineEdit->setText(filename);
    ui->lineEdit->text();
}
