# Node.js Native Add-ons from zero to hero

> Talk for Codemotion Rome 2018 
[https://rome2018.codemotionworld.com/conference/](https://rome2018.codemotionworld.com/conference/)

## Abstract

This talk is about creating Node.js interfaces for native libraries written in C 
or C++. It starts with various situations in which you need to build native addons
and the common problems in doing that. I'll discuss the reference provided by the
new N-API (Node-API) that helps mantainers to support a wide variety of Node.js 
releases without needing recompilation or abstraction layers. With all these tools
and knowledge I'll show you how to build some addons from scratch and how to 
convert existing addons using the new N-API. The last part is related to future 
developments about addons.


## [Slides](/slides)

## Examples

 - [First simple example](/00-echo)

 - [Pass function](/01-pass-function)

 - [Async worker](/02-async-worker)
 
 - [Key value database](/03-vedis-napi)

 - [Emit events](/04-addon-event-emitter)

 - [Stream and Native addon-ons](/05-addon-stream)


## C++ references

- [Programming - Principles and Practice Using C++](http://www.stroustrup.com/programming.html)

- [C++ Concurrency in Action](https://www.manning.com/books/c-plus-plus-concurrency-in-action)

## JavaScript references

- [Understanding ECMAScript 6](https://leanpub.com/understandinges6/read)

- [Exploring ES6](http://exploringjs.com/es6/)

- [Functional JavaScript](https://www.manning.com/books/functional-programming-in-javascript)

## Node.js documentation and good references

- [Node.js documentation API](https://nodejs.org/dist/latest/docs/api/)

- [Node.js Design Pattern](https://www.packtpub.com/web-development/nodejs-design-patterns-second-edition)

- [Node.js 8 the Right Way](https://pragprog.com/book/jwnode2/node-js-8-the-right-way)

- [Node Cookbook - Third Edition](https://www.packtpub.com/web-development/node-cookbook-third-edition)


### Node.js C / C ++ Addons

#### [Documenttation of Node.js Native Addons](https://nodejs.org/dist/latest/docs/api/addons.html)

### N-API 

#### [Documentation for N-API](https://nodejs.org/dist/latest/docs/api/n-api.html)

### Node Addon API
#### Header-only C++ wrapper classes for the ABI-stable Node.js API
#### [Node.js API (N-API) Package](https://github.com/nodejs/node-addon-api/)

### Web resources

* **[How I ported bcrypt to new N-API](https://medium.com/the-node-js-collection/how-i-ported-bcrypt-to-new-n-api-d0b8c9fe6136)**
* **[N-API the next API for Native Addons](https://youtu.be/-Oniup60Afs)**
* **[N-API Next generation Node API for native modules](https://www.slideshare.net/michaeldawson3572846/n-apinode-summit2017final)**
* **[Speed up Your Node.js App with Native Addons](https://medium.com/the-node-js-collection/speed-up-your-node-js-app-with-native-addons-5e76a06f4a40)**
* **[Start with N-API](https://hackernoon.com/n-api-and-getting-started-with-writing-c-addons-for-node-js-cf061b3eae75)**
* **[How to get a performance boost using Node.js native addons](https://medium.com/developers-writing/how-to-get-a-performance-boost-using-node-js-native-addons-fd3a24719c85)**

### Nicola Del Gobbo

<https://github.com/NickNaso/>

<https://www.npmjs.com/~nicknaso>

<https://twitter.com/NickNaso>

## Acknowledgements

Thank you to all people that encourage me every day.

## License

Licensed under [Apache license V2](./LICENSE)