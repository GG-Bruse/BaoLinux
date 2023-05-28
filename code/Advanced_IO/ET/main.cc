#include "TcpServer.hpp"
#include <memory>
using std::unique_ptr;

static Response calculator(const Request &req)
{
    Response resp(0, 0);
    switch (req.op_)
    {
    case '+':
        resp.result_ = req.x_ + req.y_;
        break;
    case '-':
        resp.result_ = req.x_ - req.y_;
        break;
    case '*':
        resp.result_ = req.x_ * req.y_;
        break;
    case '/':
        if (0 == req.y_)
            resp.code_ = 1;
        else
            resp.result_ = req.x_ / req.y_;
        break;
    case '%':
        if (0 == req.y_)
            resp.code_ = 2;
        else
            resp.result_ = req.x_ % req.y_;
        break;
    default:
        resp.code_ = 3;
        break;
    }
    return resp;
}

void NetCal(Connection* con, string& request) 
{
    LogMessage(DEBUG, "NetCal been call, Get request: %s", request.c_str());
    //反序列化
    Request req;
    if(!req.Deserialized(request)) return;
    //业务处理
    Response resp = calculator(req);
    //构建应答
    string sendstr = resp.Serialize();
    sendstr = Encode(sendstr);
    //递交
    con->_outBuffer += sendstr;
    //"提醒"TCP服务器处理
    con->_svrPtr->EnableReadWrite(con, true, true);
}

int main()
{
    unique_ptr<TcpServer> tcpSvrPtr(new TcpServer());
    tcpSvrPtr->Dispather(NetCal);
    return 0;
}