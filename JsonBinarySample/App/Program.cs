using Model;
using Newtonsoft.Json;
using NLECloudSDK;
using NLECloudSDK.Model;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

/*
 * 环境：
 * 1、建议使用VS2012或以上版本打开，DEMO使用.NET Framework 4.5框架编写
 * 2、App.config中ServerDomain定义云平台接口的API域名，默认为新大陆物联网云平台API域名
 * API调用步骤说明:
 * 1、云平台用户登录API,用户名与密码分别是云平台注册的帐号与密码，需一致，否则登录会失败。
 *    登录后会返回一个Token，该Token为所有其它API接口调用的凭证，所以调用其它API必须要先登录然后得到这个Token，
 *    再调用其它API时带上这个Token才能正确调用其它API
 * 2、接下来每个API的调用都会在代码中加以注释说明，请参考各个方法
 * 3、每个API会在下方分别列出请求的原始JSON格式或返回的JSON格式供参考，至于每个JSON转成对象，则看个个调用的代码
*/
namespace App
{
    class Program
    {
        
        private static String Token;         
        private static NLECloudAPI SDK = null;


        private static Int32 lineNum = 1;

        /// <summary>
        /// 控制台程序主入口
        /// </summary>
        /// <param name="args"></param>
        static void Main(string[] args)
        {
            SDK = new NLECloudAPI(Cfg.API_HOST);

            //帐号登录
            AccountLoginDTO dto = new AccountLoginDTO() { Account = Cfg.account, Password = Cfg.password };
            dynamic qry = SDK.UserLogin(dto);
            if (qry.IsSuccess())
                Token = qry.ResultObj.AccessToken;
            Console.WriteLine(lineNum + "、登录返回JSON:" + Environment.NewLine);
            Console.WriteLine(SerializeToJson(qry) + Environment.NewLine);
            lineNum++;

            if (String.IsNullOrEmpty(Token))
            {
                Console.WriteLine(lineNum + "、登录获取的Token为空，不继续执行其它接口请求，除非成功！" + Environment.NewLine);
                Console.ReadKey();
                return;
            }

            //执行读取传感数据
            new Thread(new ThreadStart(() =>
            {
                while (true)
                {
                    if (Console.ReadKey().Key == ConsoleKey.Enter)
                        QuerySensorData();
                    else if (Console.ReadKey().Key == ConsoleKey.End)
                        CmdActuator();
                }
            })).Start();

            Console.ReadKey();
        }

        /// <summary>
        /// 执行读取传感数据
        /// </summary>
        private static void QuerySensorData()
        {
            //调用批量查询设备最新数据
            ResultMsg<IEnumerable<DeviceSensorDataDTO>> qry = SDK.GetDevicesDatas(Cfg.deviceId.ToString(), Token);
            Console.WriteLine(lineNum + "、批量查询设备最新数据返回JSON:" + Environment.NewLine);
            Console.WriteLine(SerializeToJson(qry) + Environment.NewLine);
            lineNum++;

            if (qry.IsSuccess())
            {
                var dev = qry.ResultObj.FirstOrDefault();
                try
                {
                    
                    SensorDataDTO jsonSensor = dev.Datas.FirstOrDefault(item => { return item.ApiTag == Cfg.jsonApiTag; });
                    if (jsonSensor != null && jsonSensor.Value != null && jsonSensor.Value.ToString() != "")
                    {
                        //将设备传输的JSON字符串，使用JsonConvert转为对象
                        Member user = new Member();
                        user = JsonConvert.DeserializeObject<Member>(jsonSensor.Value.ToString());

                        Console.WriteLine(lineNum + "、JSON传感器值:" + Environment.NewLine);
                        Console.WriteLine(String.Format("{0}/{1}/{2}/{3}"
                            , user.UserName
                            , user.Age
                            , user.Sex
                            , user.IsMarry) + Environment.NewLine);
                        lineNum++;
                    }

                    SensorDataDTO binarySensor = dev.Datas.FirstOrDefault(item => { return item.ApiTag == Cfg.binaryApiTag; });
                    if (binarySensor != null && binarySensor.Value != null && binarySensor.Value.ToString() != "")
                    {
                        //将设备传输的Base64字符串，使用Base64解码为byte[]
                        Byte[] ary = Convert.FromBase64String(binarySensor.Value.ToString());
                        Console.WriteLine(lineNum + "、二进制传感器值:" + Environment.NewLine);
                        Console.WriteLine(String.Format("{0}/{1}/{2}/{3}"
                            , ary[0]
                            , ary[1]
                            , ary[2]
                            , ary[3]) + Environment.NewLine);
                        lineNum++;                                                                                  
                    }
                }
                catch { }
            }
        }

        /// <summary>
        /// 执行控制
        /// </summary>
        private static void CmdActuator()
        {
            //发送“JSON格式”的命令
            Member user = new Member() { UserName = "张三", Sex = 1, Age = Int32.Parse(CreateRandomNum(2)), IsMarry = false };
            String json = JsonConvert.SerializeObject(user);
            var qry = SDK.Cmds(Cfg.deviceId, Cfg.jsonActuator, json, Token);
            Console.WriteLine(lineNum + "、发送JSON格式命令返回JSON:" + Environment.NewLine);
            Console.WriteLine(SerializeToJson(qry) + Environment.NewLine);
            lineNum++;

            //发送“二进制格式”的命令
            byte[] ary = new Byte[] { 0x10, 0x09, 0x08, 0x07, 0x06 };
            String binary = Convert.ToBase64String(ary);
            qry = SDK.Cmds(Cfg.deviceId, Cfg.binaryActuator, binary, Token);
            Console.WriteLine(lineNum + "、发送二进制格式命令返回JSON:" + Environment.NewLine);
            Console.WriteLine(SerializeToJson(qry) + Environment.NewLine);
            lineNum++;
        }


        /// <summary>
        /// 将对象序列化为JSON字符串
        /// </summary>
        /// <param name="data"></param>
        /// <returns></returns>
        private static String SerializeToJson(Object data)
        {
            return Newtonsoft.Json.JsonConvert.SerializeObject(data);
        }

        /// <summary>
        /// 获取指定长度的纯数字随机数字串
        /// </summary>
        /// <param name="len"></param>
        /// <returns></returns>
        public static string CreateRandomNum(int len)
        {
            StringBuilder builder = new StringBuilder();
            byte[] data = new byte[4];
            RNGCryptoServiceProvider rng = new RNGCryptoServiceProvider();
            rng.GetBytes(data);
            Random random = new Random(BitConverter.ToInt32(data, 0));
            for (int i = 0; i < len; i++)
            {
                int num = random.Next();
                builder.Append((char)(0x30 + ((ushort)(num % 10))));
            }
            return builder.ToString();
        }
    }
}
