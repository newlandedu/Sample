using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VirGateway
{
    /// <summary>
    /// 请求类型
    /// </summary>
    public enum REQ_TYPE : byte
    {
        NONE = 0,

        /// <summary>
        /// 连接请求
        /// </summary>
        CONN_REQ = 1,

        /// <summary>
        /// 连接建立响应
        /// </summary>
        CONN_RESP = 2,

        /// <summary>
        /// 上报数据
        /// </summary>
        PUSH_DATA = 3,

        /// <summary>
        /// 上报数据确认
        /// </summary>
        PUSH_ACK = 4,

        /// <summary>
        /// 命令请求
        /// </summary>
        CMD_REQ = 5,

        /// <summary>
        /// 命令响应
        /// </summary>
        CMD_RESP = 6,

        /// <summary>
        /// 心跳请求
        /// </summary>
        PING_REQ = 7,

        /// <summary>
        /// 心跳响应
        /// </summary>
        PING_RESP = 8,

        /// <summary>
        /// 加密请求
        /// </summary>
        ENCRYPT_REQ = 9,

        /// <summary>
        /// 加密响应
        /// </summary>
        ENCRYPT_RESP = 10,
    }
}
