#pragma once

#include "exception.h"


class HTTPError
    : public Exception
{
public:
    explicit HTTPError(
        int code
        , const std::string& text
    )
        : Exception(text)
        , code_(code)
    {
    }
    virtual ~HTTPError() = default;

    int code() const noexcept {
        return code_;
    }

private:
    int code_;
};

// 4xx: Client Error
class BadRequest
    : public HTTPError
{
public:
    explicit BadRequest()
        : HTTPError(400, "Bad Request. ")
    {
    }
};

class Unauthorized
    : public HTTPError
{
public:
    explicit Unauthorized()
        : HTTPError(401, "Unauthorized. ")
    {
    }
};

class Forbidden
    : public HTTPError
{
public:
    explicit Forbidden()
        : HTTPError(403, "Forbidden. ")
    {
    }
};

class NotFound
    : public HTTPError
{
public:
    explicit NotFound()
        : HTTPError(404, "Not Found. ")
    {
    }
};

class LockedError
    : public HTTPError
{
public:
    explicit LockedError()
        : HTTPError(423, "Locked .")
    {
    }
};

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
class InternalServerError
    : public HTTPError
{
public:
    explicit InternalServerError()
        : HTTPError(500, "Internal Server Error. ")
    {
    }
};

class NotImplemented
    : public HTTPError
{
public:
    explicit NotImplemented()
        : HTTPError(501, "Not Implemented. ")
    {
    }
};
