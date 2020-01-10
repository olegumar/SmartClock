//глобальные пременные и фукция необходимые для работы
var xmlHttp=createXmlHttpObject();
function createXmlHttpObject(){
 if(window.XMLHttpRequest){
  xmlHttp=new XMLHttpRequest();
 }else{
  xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');
 }
 return xmlHttp;
}
var jsonPage;
var jsonResponse;

// фукция выполняется при запуске страницы, считывает данные из config.json
function load(){
 if(xmlHttp.readyState==0 || xmlHttp.readyState==4){
  xmlHttp.open('PUT','/config.json',true);
  xmlHttp.send(null);
  xmlHttp.onload = function(e) {
   jsonResponse=JSON.parse(xmlHttp.responseText);
   loadBlock(jsonResponse);
  } } }
// функция ищет и заменяет данные на странице из configs.json
function loadBlock(data2) {
 data = document.getElementsByTagName('body')[0].innerHTML;
 var new_string;
for (var key in data2) {
 new_string = data.replace(new RegExp('{{'+key+'}}', 'g'), data2[key]);
 data = new_string;
}
 document.getElementsByTagName('body')[0].innerHTML = new_string;
}
// находит значение(value) элемента по id маркеровке
function val(id){
 var v = document.getElementById(id).value;
 return v;
}
// функция отправляет GET запроса на сервер
function send_request(submit,server){
 request = new XMLHttpRequest();
 request.open("GET", server, true);
 request.send();
 save_status(submit,request);
}
// функция отображающая надпись на кнопке после ее нажития, до выполнея процедуры
function save_status(submit,request){
 old_submit = submit.value;
 request.onreadystatechange = function() {
  if (request.readyState != 4) return;
  submit.value = request.responseText;
  setTimeout(function(){
   submit.value=old_submit;
   submit_disabled(false);
  }, 1000);
 }
 submit.value = 'Ждем...';
 submit_disabled(true);
}
// фукция востанавливает надпись на кнопке
function submit_disabled(request){
 var inputs = document.getElementsByTagName("input");
 for (var i = 0; i < inputs.length; i++) {
  if (inputs[i].type === 'submit') {inputs[i].disabled = request;}
 } }

// фукция WebSocket, передача данных 
function run_wsocket() {
var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);
 connection.onopen = function () {
  connection.send('Connect ' + new Date());
 };
 connection.onerror = function (error) {
  console.log('WebSocket Error ', error);
 };
 connection.onmessage = function (e) {
  console.log('Server: ', e.data);
  var socket_data=JSON.parse(e.data);
  IdBlock(socket_data);
 }; }
// функция поиска по id элемента и замена текста, для работы с фукцией WebSocket
function IdBlock(wsdata) {
 var new_string;
for (var key in wsdata) {
 new_string = wsdata[key]; }
 document.getElementById([key]).innerHTML =  new_string;
 }

// ПОКА не нужна! проверяет и объединяет объекты (key : val)
function mergeObject(target) {
 for (var i = 1; i < arguments.length; i++) {
  var source = arguments[i];
  for (var key in source) {
   if (source.hasOwnProperty(key)) {
    target[key] = source[key];
   }
  }
 }
 return target;
}
// ПОКА! не нужная фукция
function toggle(target) {
 var curVal = document.getElementById(target).className;
 document.getElementById(target).className = (curVal === 'hidden') ? 'show' : 'hidden';
}

// Далее все для работы ajax обьектов
var ajax = {};
ajax.x = function () {
 var xhr;
 if (window.XMLHttpRequest) {
  xhr = new XMLHttpRequest();
 } else {
  xhr = new ActiveXObject("Microsoft.XMLHTTP");
 }
 return xhr;
};
ajax.send = function (url,callback,method,data,async) {
 if (method != 'PUT') {submit_disabled(true);}
 if (async === undefined) {
  async = true;
 }
 var x = ajax.x();
 x.open((method=='PUT'?'GET':''+method+''),url,async);
 x.onreadystatechange = function () {
  if (x.readyState == 4) {
   if (method != 'PUT') {submit_disabled(false);}
   callback(x.responseText)
  }
 };
 if (method == 'POST') {
  x.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
 }
 x.send(data)
};

ajax.get = function (url,data,callback,async) {
 var query = [];
 for (var key in data) {
  query.push(encodeURIComponent(key)+'='+encodeURIComponent(data[key]));
 }
 ajax.send(url+(query.length?'?'+query.join('&'):''),callback,'GET',null,async)
};

ajax.put = function (url,data,callback,async) {
 var query = [];
 for (var key in data) {
  query.push(encodeURIComponent(key)+'='+encodeURIComponent(data[key]));
 }
 ajax.send(url+(query.length?'?'+query.join('&'):''),callback,'PUT',null,async)
};

ajax.post = function (url,data,callback,async) {
 var query = [];
 for (var key in data) {
  query.push(encodeURIComponent(key)+'='+encodeURIComponent(data[key]));
 }
 ajax.send(url,callback,'POST',query.join('&'),async)
};
