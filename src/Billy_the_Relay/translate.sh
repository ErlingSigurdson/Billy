#! /bin/bash

read

SOURCE_1="/\*--- Включения ---\*/"
TARGET_1="/\*--- Includes ---\*/"

SOURCE_2="Project configs."
TARGET_2="Project configs."

SOURCE_3="Local modules."
TARGET_3="Local modules."

SOURCE_4="/\*\*\*\*\*\*\*\*\*\*\*\* ДИРЕКТИВЫ ПРЕПРОЦЕССОРА \*\*\*\*\*\*\*\*\*\*\*/"
TARGET_4="/\*\*\*\*\*\*\*\*\*\*\*\* PREPROCESSOR DIRECTIVES \*\*\*\*\*\*\*\*\*\*\*/"

SOURCE_5="// Command #"
TARGET_5="// Command #"

SOURCE_6="/\*\*\*\*\*\*\*\*\*\*\*\*\*\*\* ПРОТОТИПЫ ФУНКЦИЙ \*\*\*\*\*\*\*\*\*\*\*\*\*\*/"
TARGET_6="/\*\*\*\*\*\*\*\*\*\*\*\*\*\* FUNCTION PROTOTYPES \*\*\*\*\*\*\*\*\*\*\*\*\*/"

SOURCE_7="/\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\* ФУНКЦИИ \*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/"
TARGET_7="/\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\* FUNCTIONS \*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/"

SOURCE_8="// Include guards."
TARGET_8="// Include guards."

SOURCE_9="// Include guards."
TARGET_9="// Include guards."

SOURCE_10="// Additional libraries for Arduino IDE."
TARGET_10="// Additional libraries for Arduino IDE."

SOURCE_11="/\*--- Прочее ---\*/"
TARGET_11="/\*--- Misc ---\*/"

SOURCE_12="/\*\*\*\*\*\*\*\*\*\*\*\*\* ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ \*\*\*\*\*\*\*\*\*\*\*\*/"
TARGET_12="/\*\*\*\*\*\*\*\*\*\*\*\*\*\*\* GLOBAL VARIABLES \*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/"

SOURCE_13="// General Arduino library."
TARGET_13="// General Arduino library."

echo "$REPLY"

sed -i "s|$SOURCE_1|$TARGET_1|g" $REPLY
sed -i "s|$SOURCE_2|$TARGET_2|g" $REPLY
sed -i "s|$SOURCE_3|$TARGET_3|g" $REPLY
sed -i "s|$SOURCE_4|$TARGET_4|g" $REPLY
sed -i "s|$SOURCE_5|$TARGET_5|g" $REPLY
sed -i "s|$SOURCE_6|$TARGET_6|g" $REPLY
sed -i "s|$SOURCE_7|$TARGET_7|g" $REPLY
sed -i "s|$SOURCE_8|$TARGET_8|g" $REPLY
sed -i "s|$SOURCE_9|$TARGET_9|g" $REPLY
sed -i "s|$SOURCE_10|$TARGET_10|g" $REPLY
sed -i "s|$SOURCE_11|$TARGET_11|g" $REPLY
sed -i "s|$SOURCE_12|$TARGET_12|g" $REPLY
sed -i "s|$SOURCE_13|$TARGET_13|g" $REPLY
#sed -i "s|$SOURCE_14|$TARGET_14|g" $REPLY
#sed -i "s|$SOURCE_15|$TARGET_15|g" $REPLY