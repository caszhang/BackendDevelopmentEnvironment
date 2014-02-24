/*******************************************************************************
 * Author :          guoqiang
 * Email :           guoqiang@foxea.com
 * Last modified :   2014-02-24 19:17
 * Filename :        main.cc
 * Description :     
 * *****************************************************************************/
#include <stdio.h>
#include "tinyxml2.h"
using namespace std;
using namespace tinyxml2;
int main()
{
    const char *s = " <xml><ToUserName><![CDATA[gh_bc1661799282]]></ToUserName>\
            <FromUserName><![CDATA[oXK9_uO6OmP9kifwwoCnK2jGwD4c]]></FromUserName>\
            <CreateTime>1393240575</CreateTime>\
            <MsgType><![CDATA[text]]></MsgType>\
            <Content><![CDATA[Fgfhhf]]></Content>\
            <MsgId>5983922705285271635</MsgId>\
            </xml>4E4E";
    XMLDocument doc;
    doc.Parse(s, strlen(s));
    XMLElement *scene=doc.RootElement();
    printf("%s\n", scene->Value());
    const char *to_user_name = scene->FirstChildElement("ToUserName")->Value();
    printf("%s\n", to_user_name);
    const char *to_user_name_value = scene->FirstChildElement("ToUserName")->GetText();
    printf("%s\n", to_user_name_value);
    printf("-----------------\n");
    XMLElement *surfaceChild = scene->FirstChildElement();
    while (surfaceChild) {
        const char *to_user_name = surfaceChild->Value();
        printf("%s\n", to_user_name);
        const char *to_user_name_value = surfaceChild->GetText();
        printf("%s\n", to_user_name_value);
        surfaceChild = surfaceChild->NextSiblingElement();
    }
    return 0;
}
