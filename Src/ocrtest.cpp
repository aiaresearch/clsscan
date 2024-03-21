#include <iostream>
#include <curl/curl.h>
#include <string>
// libcurl库下载链接：https://curl.haxx.se/download.html
// jsoncpp库下载链接：https://github.com/open-source-parsers/jsoncpp/
const static std::string request_url = "https://aip.baidubce.com/rest/2.0/ocr/v1/general_basic";
static std::string generalBasic_result;
/**
* curl发送http请求调用的回调函数，回调函数中对返回的json格式的body进行了解析，解析结果储存在全局的静态变量当中
* @param 参数定义见libcurl文档
* @return 返回值定义见libcurl文档
*/
static size_t callback2(void *ptr, size_t size, size_t nmemb, void *stream) {
	// 获取到的body存放在ptr中，先将其转换为string格式
	generalBasic_result = std::string((char *)ptr, size * nmemb);
	return size * nmemb;
}
/**
* 通用文字识别
* @return 调用成功返回0，发生错误返回其他错误码
*/
int generalBasic(std::string &json_result, const std::string &access_token) {
	std::string url = request_url + "?access_token=" + access_token;
	CURL *curl = NULL;
	CURLcode result_code;
	int is_success;
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,FALSE);
		curl_easy_setopt(curl, CURLOPT_URL, url.data());
		curl_easy_setopt(curl, CURLOPT_POST, 1);
		curl_httppost *post = NULL;
		curl_httppost *last = NULL;
 
 
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback2);
		result_code = curl_easy_perform(curl);
		if (result_code != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(result_code));
			is_success = 1;
			return is_success;
		}
		json_result = generalBasic_result;
		curl_easy_cleanup(curl);
		is_success = 0;
	}
	else {
		fprintf(stderr, "curl_easy_init() failed.");
		is_success = 1;
	}
	return is_success;
}
 
int main()
{
	std::string stToken="24.b0c4639aeba94eb2581ad45a3296592a.2592000.1711979054.282335-48885010";
	/*get_access_token(stToken,"puMFv72xqCYV2jdcSlXgtFvm","v5XHtqKXEm0p6ehPZrXIXrftDvocyv1m");
	std::cout << "获取Token："<<stToken << std::endl;*/
 
	std::cout << "开始OCR通用识别!" << std::endl;
	std::string stBack;
	generalBasic(stBack, stToken);
	std::cout << "返回值：" << stBack << std::endl;
	return 0;
}