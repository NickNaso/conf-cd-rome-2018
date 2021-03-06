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

#include "kvdb.h"
#include <iostream>
#include <sstream>

using namespace Napi;

class GetKeyWorker: public Napi::AsyncWorker {
  public:
    GetKeyWorker(Napi::Function& callback, vedis *db, std::string cmd)
      :Napi::AsyncWorker(callback), db(db), cmd(cmd) {}
    ~GetKeyWorker() {}

    void Execute() {
      int rc;
      rc = vedis_exec(db, cmd.c_str(), -1);
      if(rc != VEDIS_OK) { 
        // Handle error
      } 
      // Extract the return value of the last executed command (i.e. 'GET test') " 
      vedis_value *result;
      vedis_exec_result(db, &result);
      // Cast the vedis object to a string 
      value = vedis_value_to_string(result, 0);
    }

    void OnOK() {
      Napi::HandleScope scope(Env()); 
      Callback().Call({ Env().Null(), Napi::String::New(Env(), value) });
    }

  private:
    vedis *db; 
    std::string cmd;
    const char *value;
};

class PutKeyWorker: public Napi::AsyncWorker {
  public:
    PutKeyWorker(Napi::Function& callback, vedis *db, std::string cmd) 
      :Napi::AsyncWorker(callback), db(db), cmd(cmd) {}
    ~PutKeyWorker() {}

    void Execute() {
      int rc;
      rc = vedis_exec(db, cmd.c_str(), -1);
      if(rc != VEDIS_OK) { 
        // Handle error
      }
    }

    void OnOK() {
      Napi::HandleScope scope(Env()); 
      Callback().Call({ Env().Null() });
    }

  private:
    vedis *db; 
    std::string cmd; 
};

void buffer_delete_callback(Env env, char* data) {
  free(data);
}

class PutKeyBufferWorker: public Napi::AsyncWorker {
  public:
    PutKeyBufferWorker(Napi::Function& callback, vedis *db, std::string key, Buffer<char> buffer)
      :AsyncWorker(callback), db(db), key(key) {
        this->buffer = buffer.Data();
        this->buffer_length = buffer.Length();
      }
    ~PutKeyBufferWorker() {}

    void Execute() {
      int rc;
      //rc = vedis_exec(db, cmd.c_str(), -1);
      // std::string s(this->buffer, this->buffer_length);
      // std::cout << "key: " << key.c_str() << " value: " << s << std::endl;
      rc = vedis_kv_store(db, key.c_str(), -1, this->buffer, this->buffer_length);
      if(rc != VEDIS_OK) { 
        // Handle error
      }
    }

    void OnOK() {
      Napi::HandleScope scope(Env()); 
      Callback().Call({ Env().Null() });
    }

  private:
    vedis *db; 
    std::string key; 
    char *buffer;
    int buffer_length;
};

// A very basic version of Node::ErrnoException
Napi::Error ErrnoException(Napi::Env env, std::string message, int errorno) {
  Napi::Error e = Napi::Error::New(env, Napi::String::New(env, message));
  e.Set("errno", Napi::Number::New(env, errno));
  e.Set("code", Napi::Number::New(env, errno));
  return e;
}

namespace KVDB {

  Napi::Object Database::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function tpl = DefineClass(env, "Database", {
      InstanceMethod("getKeySync", &Database::GetKeySync),
      InstanceMethod("putKeySync", &Database::PutKeySync),

      InstanceAccessor("db_name", &Database::DbName, nullptr),
      InstanceAccessor("root_path", &Database::RootPath, nullptr),
      
      InstanceMethod("getKey", &Database::GetKey),
      InstanceMethod("putKey", &Database::PutKey),
      InstanceMethod("putKeyBuffer", &Database::PutKeyBuffer)
    }); 
    constructor = Napi::Persistent(tpl);
    constructor.SuppressDestruct();
    exports.Set("Database", tpl);
    return exports;
  }

  Napi::Value Database::GetKey(const Napi::CallbackInfo& info) {
      // Here we need some control
      Napi::String tmpKey = info[0].As<Napi::String>();
      std::string key = tmpKey.ToString();
      std::stringstream cmd;
      cmd << "GET " + key;
      Napi::Function callback = info[1].As<Napi::Function>();
      GetKeyWorker* work = new GetKeyWorker(callback, this->db, cmd.str());
      work->Queue();
      return info.Env().Undefined();
  }

  Napi::Value Database::GetKeySync(const Napi::CallbackInfo& info) {
    // Here we need some control
    Napi::String tmpKey = info[0].As<Napi::String>();
    std::string  key =  tmpKey.ToString();
    std::stringstream cmd;
    cmd << "GET " + key;
    //KVDB::Database* database = ObjectWrap::Unwrap<KVDB::Database>(info.This());
    int rc;
    rc = vedis_exec(this->db, (cmd.str()).c_str(), -1);
    if(rc != VEDIS_OK) { 
      // Handle error
      throw ErrnoException(info.Env(), "Error executing getKeySync", rc);
    } 
    // Extract the return value of the last executed command (i.e. 'GET test') " 
    vedis_value *result;
    vedis_exec_result(this->db, &result);
    const char *value;
    // Cast the vedis object to a string 
    value = vedis_value_to_string(result, 0);
    return Napi::String::New(info.Env(), value);
  }

  Napi::Value Database::PutKey(const Napi::CallbackInfo& info) {
    // Here we need some control
    Napi::String tmpKey = info[0].As<Napi::String>();
    std::string key = tmpKey.ToString();
    Napi::String tmpValue = info[1].As<Napi::String>();
    std::string value = tmpValue.ToString();
    std::stringstream cmd;
    cmd << "SET " + key + " '" + value + "'";
    Napi::Function callback = info[2].As<Napi::Function>();
    PutKeyWorker* work = new PutKeyWorker(callback, this->db, cmd.str());
    work->Queue();
    return info.Env().Undefined();
 }

  Napi::Value Database::PutKeyBuffer(const Napi::CallbackInfo& info) {
     // Here we need some control
     Napi::String tmpKey = info[0].As<Napi::String>();
     std::string  key =  tmpKey.ToString();
     Buffer<char> value = info[1].As<Buffer<char>>();
     Napi::Function callback = info[2].As<Napi::Function>();
     PutKeyBufferWorker* work = new PutKeyBufferWorker(callback, this->db, key, value);
     work->Queue();
     return info.Env().Undefined();
  }

  Napi::Value Database::PutKeySync(const Napi::CallbackInfo& info) {
    // Here we need some control
    Napi::String tmpKey = info[0].As<Napi::String>();
    std::string  key =  tmpKey.ToString();
    Napi::String tmpValue = info[1].As<Napi::String>();
    std::string value = tmpValue.ToString();
    std::stringstream cmd;
    cmd << "SET " + key + " '" + value + "'";
    int rc;
    rc = vedis_exec(this->db, (cmd.str()).c_str(), -1);
    if (rc != VEDIS_OK) {
      // Hanlde  error
      throw ErrnoException(info.Env(), "Error executing putKeySync", rc);
    }
    return info.Env().Undefined();
  }

  Napi::Value Database::DbName(const Napi::CallbackInfo& info) {
    return Napi::String::New(info.Env(), this->db_name);
  }

  Napi::Value Database::RootPath(const Napi::CallbackInfo& info) {
    return Napi::String::New(info.Env(), this->root_path);
  }
  
  Napi::FunctionReference Database::constructor;

  Database::Database(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Database>(info) {
    // Need to check the input here
    this->db_name = info[0].As<Napi::String>();
    this->root_path = info[1].As<Napi::String>();
    std::stringstream db_path;
    db_path << root_path << "/" << db_name << db_extension;
    int rc;
    rc = vedis_open(&(this->db), (db_path.str()).c_str());
    //rc = vedis_open(&(this->db), NULL);
    createDbFolder(info);
    if (rc != VEDIS_OK) {
      // Hanlde the initialization error
      throw ErrnoException(info.Env(), "Error initializing database", rc);
    }
  }

  Database::~Database() {
    // NOOP
  }

  void Database::createDbFolder(const Napi::CallbackInfo& info) {
    int r;
    uv_fs_t req;
    r = uv_fs_stat(NULL, &req, root_path.c_str(), NULL);
    if (r != 0) {
      r = uv_fs_mkdir(NULL, &req, root_path.c_str(), 0777, NULL);
      if (r != 0) {
        uv_fs_req_cleanup(&req);
        // Handle create root folder error
        throw ErrnoException(info.Env(), "Error creating folder for database", r);
      }
    }
    uv_fs_req_cleanup(&req);
  }

}