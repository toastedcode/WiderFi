#pragma once
   
enum Mode
{
   MASTER,
   SLAVE
};

String toString(const Mode& mode);

Mode valueOf(const String& enumName);
