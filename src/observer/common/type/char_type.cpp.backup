#include <time.h>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <locale>
/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details.
*/

#include "common/lang/comparator.h"
#include "common/log/log.h"
#include "common/type/char_type.h"
#include "common/value.h"

RC CharType::cast_to(const Value &val, AttrType type, Value &result) const
{
  switch (type) {
    case AttrType::DATES: {
        RC  rc = RC::SUCCESS;
        tm tm = {};
        std::istringstream iss(val.get_string());
        iss.imbue(std::locale("zh_CN.utf-8"));
        iss >> std::get_time(&tm, "%Y-%m-%d");

        if (iss.fail()) {
          rc = RC::SCHEMA_FIELD_TYPE_MISMATCH;
        } else {
          std::time_t tt = std::mktime(&tm);
          struct tm *p = localtime(&tt);
          result.set_type(AttrType::DATES);
          result.set_year(p->tm_year + 1900);
          result.set_month(p->tm_mon + 1);
          result.set_day(p->tm_mday);
        }
        return rc;
      }
      break;
    default: return RC::UNIMPLEMENTED;
  }
  return RC::SUCCESS;
}
