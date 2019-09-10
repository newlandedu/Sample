using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VirGateway
{
    /// <summary>
    /// 连接请求
    /// </summary>
    public class PushData : BaseREQ
    {
        /// <summary>
        /// 数据上报格式类型
        /// </summary>
        public Byte datatype { get; set; }

        /// <summary>
        /// 传感数据数组
        /// </summary>
        public Object datas { get; set; }

        /// <summary>
        /// 消息编号
        /// </summary>
        public Int32 msgid { get; set; }
    }

    public class PushDatas
    {
        /// <summary>
        /// 传感标识名
        /// </summary>
        public String apitag { get; set; }

        /// <summary>
        /// 传感数据点
        /// </summary>
        public IEnumerable<DataPoint> datapoints { get; set; }
    }

    /// <summary>
    /// DataPoint
    /// </summary>
    public class DataPoint
    {
        /// <summary>
        /// 上报时间（可选）
        /// </summary>
        public String dt { get; set; }

        /// <summary>
        /// 数字浮点或字符串
        /// </summary>
        public String value { get; set; }
    }
}
