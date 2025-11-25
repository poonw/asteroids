set FILES_LIST=files_to_be_formatted.txt

@REM del %FILES_LIST%

dir /s /b src\*.cpp   >> %FILES_LIST%
dir /s /b include\*.h >> %FILES_LIST%
dir /s /b test\*.cpp  >> %FILES_LIST%

@REM where clang-format >> %FILES_LIST%
SET PATH=%PATH%;asteroids
clang-format -i -style=file --files=%FILES_LIST%

del %FILES_LIST%
