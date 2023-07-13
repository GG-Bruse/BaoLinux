#include <iostream>
#include <string>
#include <mysql.h>
using namespace std;

const string host = "43.139.44.157";
const string passwd = "123qwe@@@QWE";
const string user = "bjy";
const string db = "olinejudge";
const int port = 3306;

int main()
{
    //1、获取MySQL实例（相当于创建了一个MySQL句柄）
    MYSQL* ms = mysql_init(nullptr);

    //2、连接数据库
    if(mysql_real_connect(ms, host.c_str(), user.c_str(), passwd.c_str(), db.c_str(), port, nullptr, 0) == nullptr) {
        cerr<<"数据库连接失败!"<<endl;
        return 1;
    }
    cout<<"数据库连接成功!"<<endl;
    mysql_set_character_set(ms, "utf8"); //设置编码格式为utf8
    

    //3、查询数据库表中的记录

    //a、执行查询语句
    string sql = "select * from user";
    if(mysql_query(ms, sql.c_str()) != 0) {
        cout<<"查询数据失败!"<<endl;
        return 2;
    }
    cout<<"查询数据成功!"<<endl;
    //b、获取查询结果
    MYSQL_RES* res = mysql_store_result(ms);
    int rows = mysql_num_rows(res); //行数
    int cols = mysql_num_fields(res); //列数
    //获取每列的属性并打印列名
    MYSQL_FIELD* fields = mysql_fetch_fields(res);
    for(int i = 0;i < cols;i++) cout<<fields[i].name<<"\t";
    cout<<endl;

    for(int i = 0;i < rows;i++) {
        //获取一行数据并进行打印
        MYSQL_ROW row = mysql_fetch_row(res);
        for(int j = 0;j < cols;j++) {
            cout<<row[j]<<"\t";
        }
        cout<<endl;
    }
    free(res); //释放内存空间
    
    //4、关闭数据库
    mysql_close(ms);
    cout<<"数据库关闭成功!"<<endl;
    return 0;
}

