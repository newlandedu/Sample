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
    public class ConnREQ : BaseREQ
    {
        /// <summary>
        /// 设备标识
        /// </summary>
        public String device { get; set; }

        /// <summary>
        /// 设备传输密钥
        /// </summary>
        public String key { get; set; }

        /// <summary>
        /// 客户端固件版本号
        /// </summary>
        public String ver { get; set; }
    }
}
