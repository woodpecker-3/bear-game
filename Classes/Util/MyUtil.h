#ifndef _MYUTIL_H_
#define _MYUTIL_H_

template <class char_ptr, bool _const>
class string_parser_impl
{
public:
	string_parser_impl(char_ptr str, char sep) : _str(str), _sep(sep)
	{
	}
	char_ptr parse(char_ptr* end = NULL)
	{
		if (_str == NULL || *_str == '\0')
			return NULL;
		char_ptr begin = _str;
		while (*_str != _sep && *_str != '\0')
			_str++;
		if (end)
		{
			*end = _str;
		}
		if (*_str == _sep)
		{
			if (!_const)
				*const_cast<char*>(_str) = '\0'; //WARNING: Access violation
			_str++;
		}
		return begin;
	}
private:
	char_ptr _str;
	char _sep;
};

typedef string_parser_impl<char*, false> string_parser;
typedef string_parser_impl<const char*, true> const_string_parser;


#define TMX_WIDTH(_tileMap) ((_tileMap)->getMapSize().width*(_tileMap)->getTileSize().width)
#define TMX_HEGHT(_tileMap) ((_tileMap)->getMapSize().height*(_tileMap)->getTileSize().height)
#endif// _MYUTIL_H_
