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
    public class CmdReq : BaseREQ
    {
        /// <summary>
        /// 传感标识名
        /// </summary>
        public String apitag { get; set; }

        /// <summary>
        /// 命令ID
        /// </summary>
        public String cmdid { get; set; }

        /// <summary>
        /// 命令值
        /// </summary>
        public Object data { get; set; }
    }
}
