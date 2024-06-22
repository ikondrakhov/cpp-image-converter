# Конвертер изображений

Библиотека позволяет конвертировать изображения 3-х форматов (jpeg, bmp, ppm)


# Сборка
Для сборки проекта нужна библиотека LibJPEG

Необходимо скачать исходный код LibJPEG с [официального сайта](http://ijg.org/) и распаковать архив.

Нужно перейти в папку с `configure` файлом:

- под Winows - через Git Bash
- под Linux - через консоль

Выполните команды для сборки статической библиотеки:
```cmd
mkdir release
cd release
../configure --enable-shared=no CPPFLAGS="-O3" CFLAGS="-O3"
make
```

Если используется MinGW, то дополнительно нужно указать параметр MAKE="mingw32-make"

После завершения сборки в папке должен находиться файл libjpeg.a в папке .libs
В папке сборки также должен находиться фаил jconfig.h

Создайте папку со следующей структурой:
```
libjpeg/
|- include/
| |- jconfig.h
| |- jerror.h
| |- jmorecfg.h
| |_ jpeglib.h
|_ lib/
  |- Release
    |_ libjpeg.a
```

После этого нужно выполнить следующие команды:

```cmd
mkdir build
cd build
cmake ../ImgConverter -DCMAKE_BUILD_TYPE=Release -DLIBJPEG_DIR=<путь к пакету libjpeg>
cmake --build .
```

После этого можно запустить программу:
```
imgconv some_img.jpg some_img.ppm
```