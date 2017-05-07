#pragma once

#include "exception.h"


class HTTPError
    : public Exception
{
public:
    HTTPError(
        int code
        , const std::string& text
    )
        : Exception(text)
        , code_(code)
    {
    }

    int code() {
        return code_;
    }

private:
    int code_;
};

// 4xx: Client Error
class BadRequest
    : public HTTPError
{
    explicit BadRequest()
        : HTTPError(400, "Bad Request. ")
    {
    }
};

class Unauthorized
    : public HTTPError
{
    explicit Unauthorized()
        : HTTPError(401, "Unauthorized. ")
    {
    }
};

//      : HTTPError(403, "Forbidden («запрещено»)[2][3];
//      : HTTPError(404, "Not Found («не найдено»)[2][3];
//      : HTTPError(405, "Method Not Allowed («метод не поддерживается»)[2][3];
//      : HTTPError(406, "Not Acceptable («неприемлемо»)[2][3];
//      : HTTPError(408, "Request Timeout («истекло время ожидания»)[2][3];
//      : HTTPError(409, "Conflict («конфликт»)[2][3][4];
//      : HTTPError(410, "Gone («удалён»)[2][3];
//      : HTTPError(411, "Length Required («необходима длина»)[2][3];
//      : HTTPError(412, "Precondition Failed («условие ложно»)[2][3][7];
//      : HTTPError(413, "Request Entity Too Large («размер запроса слишком велик»)[2][3];
//      : HTTPError(414, "Request-URI Too Large («запрашиваемый URI слишком длинный»)[2][3];
//      : HTTPError(415, "Unsupported Media Type («неподдерживаемый тип данных»)[2][3];
//      : HTTPError(416, "Requested Range Not Satisfiable («запрашиваемый диапазон не достижим»)[3];
//      : HTTPError(417, "Expectation Failed («ожидаемое неприемлемо»)[3];
//      : HTTPError(422, "Unprocessable Entity («необрабатываемый экземпляр»);
//      : HTTPError(423, "Locked («заблокировано»);
//      : HTTPError(424, "Failed Dependency («невыполненная зависимость»);
//      : HTTPError(425, "Unordered Collection («неупорядоченный набор»)[8];
//      : HTTPError(426, "Upgrade Required («необходимо обновление»);
//      : HTTPError(428, "Precondition Required («необходимо предусловие»)[9];
//      : HTTPError(429, "Too Many Requests («слишком много запросов»)[9];
//      : HTTPError(431, "Request Header Fields Too Large («поля заголовка запроса слишком большие»)[9];
//      : HTTPError(444, "Закрывает соединение без передачи заголовка ответа. Нестандартный код[10];
//      : HTTPError(449, "Retry With («повторить с»)[1];
//      : HTTPError(451, "Unavailable For Legal Reasons («недоступно по юридическим причинам»)[11].
// 5xx: Server Error (ошибка сервера):
//      : HTTPError(500 Internal Server Error («внутренняя ошибка сервера»)[2][3];
//      : HTTPError(501 Not Implemented («не реализовано»)[2][3];
