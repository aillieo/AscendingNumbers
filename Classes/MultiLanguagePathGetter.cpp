#include "MultiLanguagePathGetter.h"

USING_NS_CC;

std::string MultiLanguagePathGetter::getPath()
{
	std::string tmp = "EN";

	switch (Application::getInstance()->getCurrentLanguage())
	{
	case LanguageType::CHINESE:
		tmp = "CN";
		break;
	case LanguageType::ENGLISH:
		tmp = "EN";
		break;
	default:
		break;

	}

	//tmp = "EN";
	return "multilanguage/" + tmp;
}
