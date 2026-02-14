#include "test.pb.h"
#include <iostream>
#include <string>

using namespace fix_bug;

int main(){
    // LoginResponse rsp;

    // ResultCode * rc = rsp.mutable_result();
    // rc->set_errcode(0);
    // rc->set_errmsg("登陆处理失败了");

    GetFriendListsResponse rsp;
    ResultCode * rc = rsp.mutable_result();
    rc->set_errcode(0);
    User* user1 = rsp.add_friend_list();
    user1->set_age(10);
    user1->set_name("ls");
    user1->set_sex(User::MAN);
    std::cout << rsp.friend_list_size() << std::endl;
    return 0;
}