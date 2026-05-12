#pragma once

#include <cstdint>
#include <cstdio>

struct binary_stream
{
    virtual ~binary_stream() {}

    virtual void close() = 0;
};

struct binary_in_stream : public binary_stream
{
    virtual std::size_t read(void *buffer, std::size_t bytes)        = 0;
};

struct binary_out_stream : public binary_stream
{
    virtual std::size_t write(const void *buffer, std::size_t bytes) = 0;
    virtual void        flush()                                      = 0;
};

inline bool write(binary_out_stream &s, const char *message)
{
    size_t size = strlen(message);
    return s.write(message, size) == size;
}

inline bool write(binary_out_stream &s, const std::string &message)
{
    return s.write(message.data(), message.size()) == message.size();
}

bool write_format(binary_out_stream &s, const char *fmt, ...);


class StringOutStream : public IOutStream
{
    std::string m_str;

public:

    size_t             write(const void *u, size_t bytes) { m_str.append((const char *)u, bytes); return bytes; }
    void               close() {}
    void               flush() {}
    const std::string &str() const { return m_str; }    
};

// Buffered implementations of IInStream and IOutStream
class FileInStream : public IInStream
{
    FILE *m_file;

public:

    FileInStream(FILE *file);
    FileInStream(const char *path);

    size_t read(void *u, size_t bytes);
    void   close();
};

class FileOutStream : public IOutStream
{
    FILE *m_file;

public:

    FileOutStream(FILE *file);
    FileOutStream(const char *path);

    size_t write(const void *u, size_t bytes);
    void   close();
    void   flush();
};

class StdOutStream : public FileOutStream
{
public:
    StdOutStream(FILE *file) : FileOutStream(file) {}
    void close() {}
};

#endif  // _STREAMS_IINSTREAM_IOUTSTREAM_H
