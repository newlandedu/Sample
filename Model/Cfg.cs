using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Model
{
    public class Cfg
    {
        //此为“应用开发”用到的参数
        public static String API_HOST = "http://192.168.0.6:8080";     //开发测试用，替换成正式地址 http://api.nlecloud.com

        public static String account = "18965562233";                   //开发测试用，云平台登录帐号
        public static String password = "123456";                       //开发测试用，云平台登录密码
        public static Int32 deviceId = 1128;                            //开发测试用，云平上面的项目下的设备ID
        public static String jsonApiTag = "jsonsensor";                 //开发测试用，Json值的传感标识
        public static String binaryApiTag = "binarysensor";             //开发测试用，二进制值的传感标识
        public static String jsonActuator = "jsonactuator";             //开发测试用，Json值的执行器标识名
        public static String binaryActuator = "binaryactuator";         //开发测试用，二进制值的执行器标识名


        //此为“设备接入”用到的参数
        public static String nleCloudIP = "192.168.0.6";                //开发测试用，替换成正式IP 120.77.58.34
        public static Int32 nleCloudPort = 8600;
        public static String deviceTag = "virgateway002";               //开发测试用，云平上面的项目下的设备ID
        public static String key = "70e5f478807e4e2bbca85959c0753a2e";  //开发测试用，替换成你的设备密钥
    }
}
