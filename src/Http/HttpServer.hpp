/*
* @file_name: http_server.hpp
* @date: 2021/12/06
* @author: oaho
* Copyright @ hz oaho, All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#ifndef FOLLOWVARY_HTTPSERVER_HPP
#define FOLLOWVARY_HTTPSERVER_HPP
#include "HttpSession.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Util/util.h"
#include <Network/TcpServer.h>
#include <functional>
#include <memory>
#include <map>
#include <mutex>
namespace http{
    class http_server: protected toolkit::TcpServer, public toolkit::noncopyable{
    public:
        using Ptr = std::shared_ptr<http_server>;
        using http_callback = std::function<void(std::shared_ptr<HttpSession>)>;
    public:
        http_server(const toolkit::EventPoller::Ptr &poller = nullptr): toolkit::TcpServer(poller){}
    public:
        /**
         * port: 启动的端口
         * host: ip
         * backlog: listen时的backlog数
         * */
        void start(uint16_t port, const std::string &host = "0.0.0.0", uint32_t backlog = 1024);

        /*
         * path: http path路径
         * callback: 回调
         * 开放一个http接口，线程不安全
         * */
        void api_regist(const std::string& path, http_callback&& callback){
            api_invokes.operator[](path) = std::move(callback);
        }
        http_callback* get_invoke(const std::string& path){
            static http_callback _invokes = nullptr;
            auto it = api_invokes.find(path);
            if( it != api_invokes.end()){
                return &(*it).second;
            }
            return &_invokes;
        }
    private:
        std::map<std::string, http_callback> api_invokes;
    };
};


#endif//FOLLOWVARY_HTTPSERVER_HPP