#pragma once

#include "exception.h"


namespace Lib {
namespace HTTP {

class Error
    : public Exception
{
public:
    explicit Error(
        int code
        , const std::string& text
    )
        : Exception(text)
        , code_(code)
    {
    }
    virtual ~Error() = default;

    int code() const noexcept {
        return code_;
    }

private:
    int code_;
};

// 4xx: Client Error
class BadRequestError
    : public Error
{
public:
    explicit BadRequestError()
        : Error(400, "Bad Request. ")
    {
    }
};

class UnauthorizedError
    : public Error
{
public:
    explicit UnauthorizedError()
        : Error(401, "Unauthorized. ")
    {
    }
};

class ForbiddenError
    : public Error
{
public:
    explicit ForbiddenError()
        : Error(403, "Forbidden. ")
    {
    }
};

class NotFoundError
    : public Error
{
public:
    explicit NotFoundError()
        : Error(404, "Not Found. ")
    {
    }
};

class LockedError
    : public Error
{
public:
    explicit LockedError()
        : Error(423, "Locked .")
    {
    }
};

class MethodNotAllowedError
    : public Error
{
public:
    explicit MethodNotAllowedError()
        : Error(405, "Method Not Allowed. ")
    {
    }
};

class NotAcceptableError
    : public Error
{
public:
    explicit NotAcceptableError()
        : Error(406, "Not Acceptable. ")
    {
    }
};

class RequestTimeoutError
    : public Error
{
public:
    explicit RequestTimeoutError()
        : Error(408, "Request Timeout. ")
    {
    }
};

class ConflictError
    : public Error
{
public:
    explicit ConflictError()
        : Error(409, "Conflict. ")
    {
    }
};

class LengthRequiredError
    : public Error
{
public:
    explicit LengthRequiredError()
        : Error(411, "Length Required. ")
    {
    }
};

class PreconditionFailedError
    : public Error
{
public:
    explicit PreconditionFailedError()
        : Error(412, "Precondition Failed. ")
    {
    }
};

class RequestEntityTooLargeError
    : public Error
{
public:
    explicit RequestEntityTooLargeError()
        : Error(413, "Request Entity Too Large. ")
    {
    }
};

class RequestURITooLargeError
    : public Error
{
public:
    explicit RequestURITooLargeError()
        : Error(414, "Request-URI Too Large. ")
    {
    }
};

class UnsupportedMediaTypeError
    : public Error
{
public:
    explicit UnsupportedMediaTypeError()
        : Error(415, "Unsupported Media Type. ")
    {
    }
};

class RequestedRangeNotSatisfiableError
    : public Error
{
public:
    explicit RequestedRangeNotSatisfiableError()
        : Error(416, "Requested Range Not Satisfiable. ")
    {
    }
};

class ExpectationFailedError
    : public Error
{
public:
    explicit ExpectationFailedError()
        : Error(417, "Expectation Failed. ")
    {
    }
};

class UnprocessableEntityError
    : public Error
{
public:
    explicit UnprocessableEntityError()
        : Error(422, "Unprocessable Entity. ")
    {
    }
};

class FailedDependencyError
    : public Error
{
public:
    explicit FailedDependencyError()
        : Error(424, "Failed Dependency. ")
    {
    }
};

class UnorderedCollectionError
    : public Error
{
public:
    explicit UnorderedCollectionError()
        : Error(425, "Unordered Collection. ")
    {
    }
};

class UpgradeRequiredError
    : public Error
{
public:
    explicit UpgradeRequiredError()
        : Error(426, "Upgrade Required. ")
    {
    }
};

class PreconditionRequiredError
    : public Error
{
public:
    explicit PreconditionRequiredError()
        : Error(428, "Precondition Required. ")
    {
    }
};

class TooManyRequestsError
    : public Error
{
public:
    explicit TooManyRequestsError()
        : Error(429, "Too Many Requests. ")
    {
    }
};

class RequestHeaderFieldsTooLargeError
    : public Error
{
public:
    explicit RequestHeaderFieldsTooLargeError()
        : Error(431, "Request Header Fields Too Large. ")
    {
    }
};

class RetryWithError
    : public Error
{
public:
    explicit RetryWithError()
        : Error(449, "Retry With. ")
    {
    }
};


// 5xx: Server Error
class InternalServerError
    : public Error
{
public:
    explicit InternalServerError()
        : Error(500, "Internal Server Error. ")
    {
    }
};

class NotImplementedError
    : public Error
{
public:
    explicit NotImplementedError()
        : Error(501, "Not Implemented. ")
    {
    }
};

}  // namespace HTTP
}  // namespace Lib
