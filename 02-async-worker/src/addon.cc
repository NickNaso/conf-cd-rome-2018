/*******************************************************************************
 * Copyright (c) 2018 Nicola Del Gobbo
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the license at http://www.apache.org/licenses/LICENSE-2.0
 *
 * THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR CONDITIONS
 * OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION ANY
 * IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS FOR A PARTICULAR PURPOSE,
 * MERCHANTABLITY OR NON-INFRINGEMENT.
 *
 * See the Apache Version 2.0 License for specific language governing
 * permissions and limitations under the License.
 *
 * Contributors - initial API implementation:
 * Nicola Del Gobbo <nicoladelgobbo@gmail.com>
 ******************************************************************************/

#include<napi.h>

#include <chrono>
#include <thread>
#include <iostream>

class EchoWorker : public Napi::AsyncWorker {
    public:
        EchoWorker(Napi::Function& callback, std::string& echo)
        : Napi::AsyncWorker(callback), echo(echo) {}

        ~EchoWorker() {}
    
    // This code will be executed on the worker thread
    void Execute() {
        // Need to simulate cpu heavy task
        std::this_thread::sleep_for(std::chrono::seconds(1));
        // std::cout << echo << std::endl;
    }  

    void OnOK() {
        Napi::HandleScope scope(Env());
        Callback().Call({Env().Null(), Napi::String::New(Env(), echo)});
    }

    private:
        std::string echo;    
};

Napi::Value Echo(const Napi::CallbackInfo& info) {
    // You need to check the input data here
    Napi::Function cb = info[1].As<Napi::Function>();
    std::string in = info[0].As<Napi::String>();
    EchoWorker* wk = new EchoWorker(cb, in);
    wk->Queue(); 
    return info.Env().Undefined();  
}

// Init
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "echo"), Napi::Function::New(env, Echo));
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init);