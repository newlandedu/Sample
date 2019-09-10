using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VirGateway
{
    /// <summary>
    /// 连接响应类型
    /// </summary>
    public enum CONN_RESP_TYPE : byte
    {
        /// <summary>
        /// 连接成功
        /// </summary>
        RESP_SUCCESS = 0,

        /// <summary>
        /// 握手失败-协议错误
        /// </summary>
        RESP_PROTOCOL_ERROR = 1,

        /// <summary>
        /// 握手失败-未添加设备
        /// </summary>
        RESP_UNADDED_DEVICE = 2,

        /// <summary>
        /// 握手失败-设备鉴权失败
        /// </summary>
        RESP_AUTH_FAILED = 3,

        /// <summary>
        /// 握手失败-未授权
        /// </summary>
        RESP_UNAUTHORIZED = 4,
    }
}
