using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VirGateway
{
    /// <summary>
    /// 连接响应
    /// </summary>
    public class ConnRESP : BaseREQ
    {
        /// <summary>
        /// 状态结果
        /// </summary>
        public CONN_RESP_TYPE status { get; set; }

    }
}
