# [NCKU Data Mining project](http://class-qry.acad.ncku.edu.tw/syllabus/online_display.php?syear=0109&sem=1&co_no=P764600&class_code=)

## requirement

* cmake 3
* c++ compiler with c++17 support
* python3
    * pandas
    * matplotlib
    * numpy

## install

```bash
git clone https://github.com/StoneLin0708/DataMining2020.git
```

If you don't have cmake

```bash
pip install cmake
```

## build

```bash
git submodule update --init
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

## apriori

```bash
./build/src/apriori data/ibm.csv 0.01 0.6
```

Disable output and compute time usage (i7-8700)

```bash
time ./build/src/apriori data/ibm.csv 0.01 0.6 0
./build/src/apriori data/ibm.csv 0.01 0.6 0  0.92s user 0.00s system 99% cpu 0.917 total
```

## fp growth

```bash
./build/src/fpg data/ibm.csv 0.01 0.6
```

Disable output and compute time usage (i7-8700)

```bash
time ./build/src/fpg data/ibm.csv 0.01 0.6 0
./build/src/fpg data/ibm.csv 0.01 0.6 0  0.06s user 0.01s system 99% cpu 0.074 total
```

## benchmark

```
python scripts/bench.py
```

## kaggle dataset

* source [movielens](https://www.kaggle.com/jneupane12/movielens?select=ratings.csv)

```bash
python scripts/run_movie.py
```