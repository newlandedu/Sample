using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VirGateway
{
    /// <summary>
    /// 命令响应
    /// </summary>
    public class CmdResp : BaseREQ
    {
        /// <summary>
        /// 命令ID
        /// </summary>
        public String cmdid { get; set; }

        /// <summary>
        /// 命令响应状态
        /// </summary>
        public Byte status { get; set; }

        /// <summary>
        /// 命令响应值
        /// </summary>
        public String data { get; set; }
    }
}
