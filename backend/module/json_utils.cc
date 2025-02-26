#include "json_utils.h"

Json::Value *json_parse(Json::Value &root, const std::string &message)
{
  try
  {
    Json::CharReaderBuilder builder;
    builder["collectComments"] = true;
    JSONCPP_STRING errs;
    auto reader = builder.newCharReader();
    if (!reader->parse(message.c_str(), message.cend().base(), &root, &errs))
    {
      throw "TODO Err";
    }
    return &root;
  }
  catch (...)
  {
    ERR_LOGGER("Json", "purse failure");
    throw "TODO Err";
  }
}

std::string json_stringify(Json::Value &json_value)
{
  Json::FastWriter fastWriter;
  return fastWriter.write(json_value);
};
