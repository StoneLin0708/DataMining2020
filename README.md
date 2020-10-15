# [NCKU Data Mining project](http://class-qry.acad.ncku.edu.tw/syllabus/online_display.php?syear=0109&sem=1&co_no=P764600&class_code=)

## requirement

* cmake 3
* compiler with c++17 support

## build

```bash
git submodule init --update
mkdir build
cd build
cmake ..
make
```

## apriori

```bash
./build/src/apriori data/ex0.csv 10
```

## fp growth

```bash
./build/src/fpg data/ex0.csv 10
```

