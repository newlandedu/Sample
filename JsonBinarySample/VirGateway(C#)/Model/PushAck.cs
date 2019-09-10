using Comm.Utils;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VirGateway
{
    /// <summary>
    /// 上报数据确认
    /// </summary>
    public class PushAck : BaseREQ
    {
        /// <summary>
        /// 消息编号
        /// </summary>
        public Int32 msgid { get; set; }

        /// <summary>
        /// 状态结果0：上报成功；1：上报失败；
        /// </summary>
        public ResultStatus status { get; set; }
    }
}
