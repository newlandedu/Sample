using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Comm.Utils;
using VirGateway;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using Model;

namespace VirGateway
{
    /// <summary>
    /// 程序
    /// </summary>
    class Program
    {
        private static TcpClient client;

        /// <summary>
        /// 程序主入口
        /// </summary>
        /// <param name="args"></param>
        static void Main(string[] args)
        {
            client = new TcpClient(Cfg.nleCloudIP, Cfg.nleCloudPort, Encoding.UTF8);
            client.ConnectedServer += Client_ConnectedServer;
            client.DisConnectedServer += Client_DisConnectedServer;
            client.ReceivedDatagram += Client_ReceivedDatagram;
            client.Connect();

            Console.ReadLine();
        }

        /// <summary>
        /// 接收信息回调
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private static void Client_ReceivedDatagram(object sender, NetReceiveEventArgs e)
        {
            String datagram = e.Datagram;

            JsonSerializerSettings jsonSettings = new JsonSerializerSettings();
            jsonSettings.Converters.Add(new REQConverter());
            BaseREQ req = JsonConvert.DeserializeObject<BaseREQ>(datagram, jsonSettings);
            if (req == null)
                return;


            switch (req.t)
            {
                case REQ_TYPE.CONN_RESP://连接响应
                    {
                        if (req is ConnRESP && ((ConnRESP)req).status == CONN_RESP_TYPE.RESP_SUCCESS)
                        {
                            StartSendThread();
                        }
                    }
                    break;

                case REQ_TYPE.PUSH_ACK://上传数据响应
                    {
                        if (req is PushAck && ((PushAck)req).status == ResultStatus.Success)
                        {
                            PushAck ack = (PushAck)req;
                        }
                    }
                    break;

                case REQ_TYPE.CMD_REQ://命令请求
                    {
                        if (req is CmdReq)
                        {
                            CmdReq cmd = req as CmdReq;

                            if(cmd.data != null && cmd.data.ToString() != "")
                            {
                                //接收发过来的JSON格式命令，使用JsonConvert反序列
                                if (cmd.apitag == Cfg.jsonActuator)
                                {
                                    Member user = JsonConvert.DeserializeObject<Member>(cmd.data.ToString());
                                    Console.WriteLine("JSON格式命令:" + Environment.NewLine);
                                    Console.WriteLine(String.Format("{0}/{1}/{2}/{3}"
                                        , user.UserName
                                        , user.Age
                                        , user.Sex
                                        , user.IsMarry) + Environment.NewLine);
                                }
                                //接收发过来的二进制格式命令，使用Base64解码
                                else if (cmd.apitag == Cfg.binaryActuator)
                                {
                                    Byte[] ary = Convert.FromBase64String(cmd.data.ToString());
                                    Console.WriteLine("二进制格式命令:" + Environment.NewLine);
                                    Console.WriteLine(String.Format("{0}/{1}/{2}/{3}"
                                        , ary[0]
                                        , ary[1]
                                        , ary[2]
                                        , ary[3]) + Environment.NewLine);
                                }
                            }

                            CmdResp resp = new CmdResp()
                            {
                                cmdid = cmd.cmdid,
                                t = REQ_TYPE.CMD_RESP,
                                status = (Byte)ResultStatus.Success
                            };
                            client.SendText(Newtonsoft.Json.JsonConvert.SerializeObject(resp));
                        }
                    }
                    break;
            }
        }

        /// <summary>
        /// 发送传感数据线程
        /// </summary>
        private static void StartSendThread()
        {
            new Thread(new ThreadStart(()=> {
                while (client != null && client.IsConnected)
                {
                    PushData datas = new PushData()
                    {
                        t = REQ_TYPE.PUSH_DATA,
                        msgid = TextUtility.CreateRandomNum(5).ToInt32(),
                        datatype = 1,
                    };


                    //传输JSON值的关键代码示例，比如要传输一个Member对象，可实例一个Member对象user,再将之序列号为JSON字符串然后附值给apiTag
                    Member user = new Member() { UserName = "张三", Sex = 1, Age = TextUtility.CreateRandomNum(2).ToInt32(), IsMarry = false };
                    var J = new JObject();
                    J.Add(Cfg.jsonApiTag, JsonConvert.SerializeObject(user));

                    //传输二进值的关键代码示例，将byte[]使用Base64编码为字符串
                    Byte[] ary = new Byte[] { 0x01, 0x02, 0x03, 0x04, 0x05 };
                    J.Add(Cfg.binaryApiTag, Convert.ToBase64String(ary));

                    datas.datas = J;
                    try
                    {
                        //发送给云平台
                        String datagram = JsonConvert.SerializeObject(datas);
                        client.SendText(datagram);
                    }
                    catch { }

                    Thread.Sleep(5000);
                }
            })).Start();
            
        }

        /// <summary>
        /// 断开连接回调事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private static void Client_DisConnectedServer(object sender, NetEventArgs e)
        {
            client = null;
        }

        /// <summary>
        /// 成功连接回调事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private static void Client_ConnectedServer(object sender, NetEventArgs e)
        {
            //连接成功后发送 “握手信息”
            ConnREQ connREQ = new ConnREQ()
            {
                t = REQ_TYPE.CONN_REQ,
                device = Cfg.deviceTag,
                key = Cfg.key,
                ver = "v3.0.2"
            };
            String strJson = JsonConvert.SerializeObject(connREQ);
            client.SendText(strJson);
        }
    }
}
