#ifndef ARPACTDEV_H
#define ARPACTDEV_H


#include <stable.h>
#include <QWidget>
#include "arpsendthread.h"
#include "arpgetthread.h"

#ifndef IPTOSBUFFERS
#define IPTOSBUFFERS    12
#endif
#ifndef ETH_ARP
#define ETH_ARP         0x0806  //以太网帧类型表示后面数据的类型，对于ARP请求或应答来说，该字段的值为x0806
#endif
#ifndef ARP_HARDWARE
#define ARP_HARDWARE    1  //硬件类型字段值为表示以太网地址
#endif
#ifndef ETH_IP
#define ETH_IP          0x0800  //协议类型字段表示要映射的协议地址类型值为x0800表示IP地址
#endif
#ifndef ARP_REQUEST
#define ARP_REQUEST     1   //ARP请求
#endif
#ifndef ARP_REPLY
#define ARP_REPLY       2      //ARP应答
#endif
#ifndef HOSTNUM
#define HOSTNUM         255   //主机数量
#endif

#pragma pack(1)
//帧头部结构体，共14字节
struct EthernetHeader
{
    u_char DestMAC[6];    //目的MAC地址 6字节
    u_char SourMAC[6];   //源MAC地址 6字节
    u_short EthType;         //上一层协议类型，如0x0800代表上一层是IP协议，0x0806为arp  2字节
};

//28字节ARP帧结构
struct Arpheader {
    unsigned short HardwareType; //硬件类型
    unsigned short ProtocolType; //协议类型
    unsigned char HardwareAddLen; //硬件地址长度
    unsigned char ProtocolAddLen; //协议地址长度
    unsigned short OperationField; //操作字段
    unsigned char SourceMacAdd[6]; //源mac地址
    unsigned long SourceIpAdd; //源ip地址
    unsigned char DestMacAdd[6]; //目的mac地址
    unsigned long DestIpAdd; //目的ip地址
};

//arp包结构
struct ArpPacket {
    EthernetHeader ed;
    Arpheader ah;
};

struct sparam {
    pcap_t *adhandle;
    char *ip;
    unsigned char *mac;
    char *netmask;
};
struct gparam {
    pcap_t *adhandle;
};


namespace Ui {
class arpactdev;
}

class arpactdev : public QWidget
{
    Q_OBJECT

public:
    explicit arpactdev(QWidget *parent = 0);
    ~arpactdev();

private:
    Ui::arpactdev *ui;
public:
    char *ip_addr;                                    //IP地址
    char *ip_netmask;                             //子网掩码
    unsigned char *ip_mac;          //本机MAC地址
    QHash<QString,QString> actmac;      //活动设备的mac地址

    bool flag;                      //ip地址是否可用的标志
    pcap_if_t *alldevs;
    char errbuf[PCAP_ERRBUF_SIZE];
    int dev_num;                            //适配器计数变量
    pcap_if_t *d;
    QVector <pcap_if_t *> usabledev;
    pcap_t *adhandle;           //捕捉实例,是pcap_open返回的对象

    int init();
    QString iptos(u_long in);       //转换ip地址为qstring 格式
    QString mactos(unsigned char* mac);  //转换mac地址为qstring 格式
    void ifget(pcap_if_t *d, char *ip_addr, char *ip_netmask);
    int GetSelfMac(pcap_t *adhandle, const char *ip_addr, unsigned char *ip_mac);
    void upmactab();            //更新活动主机ip及mac显示

private slots:
    void on_ipcombox_currentIndexChanged(const QString &arg1);
    void on_label_linkActivated(const QString &link);
    void on_getmacbutton_clicked();
    void getactmac(unsigned long ip,QString mac);  //接收局域网内活动主机mac信号，更新actmac

    void on_pushButton_clicked();

private:
    arpsendthread arpth;                //发送arp探测包的线程
    arpgetthread arpgth;                //获取arp探测包的线程

};
#endif // ARPACTDEV_H
