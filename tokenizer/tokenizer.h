#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "error/error.h"
#include "tokenizer/token.h"
#include "tokenizer/utils.hpp"

namespace miniplc0 {

class Tokenizer final {
 private:
  using uint64_t = std::uint64_t;

  // ״̬��������״̬
  enum DFAState {
    INITIAL_STATE,
    UNSIGNED_INTEGER_STATE,
    PLUS_SIGN_STATE,
    MINUS_SIGN_STATE,
    DIVISION_SIGN_STATE,
    MULTIPLICATION_SIGN_STATE,
    IDENTIFIER_STATE,
    EQUAL_SIGN_STATE,
    SEMICOLON_STATE,
    LEFTBRACKET_STATE,
    RIGHTBRACKET_STATE
  };

 public:
  Tokenizer(std::istream &ifs)
      : _rdr(ifs), _initialized(false), _ptr(0, 0), _lines_buffer() {}
  Tokenizer(Tokenizer &&tkz) = delete;
  Tokenizer(const Tokenizer &) = delete;
  Tokenizer &operator=(const Tokenizer &) = delete;

  // ���ĺ�����������һ�� token
  std::pair<std::optional<Token>, std::optional<CompilationError>> NextToken();
  // һ�η������� token
  std::pair<std::vector<Token>, std::optional<CompilationError>> AllTokens();

 private:
  // ��� Token �ĺϷ���
  std::optional<CompilationError> checkToken(const Token &);
  //
  // ** ����Ҫ���������� **
  //
  // ������һ�� token���� NextToken ʵ��ʵ�ֲ���
  std::pair<std::optional<Token>, std::optional<CompilationError>> nextToken();

  // �����￪ʼ��ʵ��һ�������кŵĻ�������ʵ��
  // Ϊ�˼����������û�е����ó�һ����ʵ��
  // ����˼��� C ���ļ�����������ƣ�����һ�� buffer ��һ��ָ�룬������ϸ��
  // 1.���������� \n
  // 2.ָ��ʼ��ָ����һ��Ҫ��ȡ�� char
  // 3.�кź��кŴ� 0 ��ʼ

  // һ�ζ���ȫ�����ݣ������滻���л���Ϊ \n
  // ������ʵ�ǲ�����ģ�����ֻ�Ǽ������ôʵ��
  void readAll();
  // һ���򵥵��ܽ�
  // | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9  | ƫ��
  // | = | = | = | = | = | = | = | = | = | =  |
  // | h | a | 1 | 9 | 2 | 6 | 0 | 8 | 1 | \n |����������0�У�
  // | 7 | 1 | 1 | 4 | 5 | 1 | 4 |             ����������1�У�
  // �������ָ��ָ���һ�е� \n����ô��
  // nextPos() = (1, 0)
  // currentPos() = (0, 9)
  // previousPos() = (0, 8)
  // nextChar() = '\n' ����ָ���ƶ��� (1, 0)
  // unreadLast() ָ���ƶ��� (0, 8)
  std::pair<uint64_t, uint64_t> nextPos();
  std::pair<uint64_t, uint64_t> currentPos();
  std::pair<uint64_t, uint64_t> previousPos();
  std::optional<char> nextChar();
  bool isEOF();
  void unreadLast();

 private:
  std::istream &_rdr;
  // ���û�г�ʼ������ô�� readAll
  bool _initialized;
  // ָ����һ��Ҫ��ȡ���ַ�
  std::pair<uint64_t, uint64_t> _ptr;
  // ����Ϊ�����Ļ�����
  std::vector<std::string> _lines_buffer;
};
}  // namespace miniplc0

