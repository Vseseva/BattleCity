# BattleCity
# Порядок установки на ОС Linux:

1. Открыть терминал в папке с проектом
2. Установить SFML
  mkdir SFML
  cd SFML
  wget https://www.sfml-dev.org/files/SFML-2.6.1-windows-vc17-64-bit.zip
  unzip SFML-2.6.1-windows-vc17-64-bit.zip
3. Устанавливаем cmake
  sudo apt-get install cmake
4. Устанавливаем зависимости
   sudo apt-get install xcb-image
   sudo apt-get install libgl1-mesa-dev
   sudo apt-get install mesa-utils
   sudo apt-get install libxcb-image0-dev
   sudo apt-get install libudev-dev
   sudo apt-get install libjpeg-dev
   sudo apt-get install libopenal-dev
   sudo apt-get install libflac-dev
   sudo apt-get install libvorbis-dev
5. Выполнить сборку cmake
   cd ..
   cmake -DCMAKE_BUILD_TYPE=release -DBUILD_STATIC_LIBS=ON -DBUILD_SHARED_LIBS=ON -DARCHIVE_INSTALL_DIR=. -G "Unix Makefiles"
6. Выполнить компиляцию программы
   sudo make && make install
7. В папку с исполняемым файлом скинуть папку images
