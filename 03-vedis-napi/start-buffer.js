/*******************************************************************************
 * Copyright (c) Nicola Del Gobbo
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

'use strict'

const { Database } = require('bindings')('kvdb')

const buffer = Buffer.from('qwertyuiopasdfghjklzxcvbnm1234567890')

const mydb = new Database('test', './tmp-buffer')

console.log(mydb.db_name)

mydb.putKeyBuffer("image", buffer, function (err) {
    if (err) {
        console.error("Error happened storing buffer value for key -> image")
    } else {
        console.log("Buffer value for key -> image successfully stored")
        console.log(mydb.getKeySync('image'))
    }
})