using VirGateway;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VirGateway
{
    /// <summary>
    /// REQConverter
    /// </summary>
    public class REQConverter : JsonConverter
    {
        /// <summary>
        /// CanConvert
        /// </summary>
        /// <param name="objectType"></param>
        /// <returns></returns>
        public override bool CanConvert(Type objectType)
        {
            return true;
        }

        /// <summary>
        /// WriteJson
        /// </summary>
        /// <param name="writer"></param>
        /// <param name="value"></param>
        /// <param name="serializer"></param>
        public override void WriteJson(JsonWriter writer, object value, JsonSerializer serializer)
        {

        }

        /// <summary>
        /// ReadJson方法定义的是Json.Net用来处理反序列化的逻辑
        /// </summary>
        public override object ReadJson(JsonReader reader, Type objectType, Object existingValue, JsonSerializer serializer)
        {
            BaseREQ result = null;
            JObject jObject = null;

            try
            {
                jObject = JObject.Load(reader);
            }
            catch
            {
                return result;
            }

            REQ_TYPE t = REQ_TYPE.NONE;
            try
            {
                Enum.TryParse<REQ_TYPE>(jObject["t"] == null ? "" : jObject["t"].ToString(), out t);
                if (t == REQ_TYPE.NONE)
                    return result;

                switch (t)
                {
                    case REQ_TYPE.CONN_RESP:
                        {
                            String strJson = JsonConvert.SerializeObject(jObject);
                            result = JsonConvert.DeserializeObject<ConnRESP>(strJson);
                        }
                        break;

                    case REQ_TYPE.PUSH_ACK:
                        {
                            String strJson = JsonConvert.SerializeObject(jObject);
                            result = JsonConvert.DeserializeObject<PushAck>(strJson);
                        }
                        break;

                    case REQ_TYPE.CMD_REQ:
                        {
                            String strJson = JsonConvert.SerializeObject(jObject);
                            result = JsonConvert.DeserializeObject<CmdReq>(strJson);
                        }
                        break;
                }
            }
            finally
            {
                //以上的一些字段的类型解析错误会为空，则根据t初始BaseREQ类型
                if (result == null)
                    result = new BaseREQ() { t = t };
            }

            return result;
        }
    }
}
