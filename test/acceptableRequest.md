Request = Request-Line
          *((general-header | request-header | entity-header) CRLF)
          CRLF
          [message-body]

Request-Line = Method SP Request-URI SP HTTP-Version CRLF

Method = "GET" | "POST" | "DELETE"

Request-URI = abs_path
  description:
    abs_path is a part of http-URL. http-URL is a form of { "http:" "//" host [":" port] [abs_path ["?" query]] }.

HTTP-Version = "HTTP" "/" 1*DIGIT "." 1*DIGIT
