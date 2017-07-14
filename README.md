# DSGDpp

This project contains implementations of various parallel algorithms for computing low-rank matrix factorizations. Both shared-memory and shared-nothing (via MPI) implementations are provided. 

The following algorithms are currently implemented:

|Algorithm|Reference|Shared memory?|Shared nothing?|
|---|---|:---:|:---:|
|SGD||||
|DSGD|[Gemulla et al., 2011](https://dl.acm.org/citation.cfm?id=2020426)|&#10003;|&#10003;|
|DSGD++|[Teflioudi et al., 2012](http://ieeexplore.ieee.org/document/6413862)|&#10003;|&#10003;|
|Asynchronous SGD|[Teflioudi et al., 2012](http://ieeexplore.ieee.org/document/6413862)||&#10003;|
|CSGD|[Makari et al., 2013](https://link.springer.com/article/10.1007/s10115-013-0718-7)|&#10003;||
|Parallel SGD with locking||&#10003;|
|Lock-free parallel SGD (Hogwild)|[Niu et al., 2011](https://dl.acm.org/citation.cfm?id=2986537)|&#10003;||
|Alternating least squares|[Zhou et al., 2008](https://link.springer.com/chapter/10.1007/978-3-540-68880-8_32)|&#10003;|&#10003;|
|Non-negative matrix factorization|[Lee and Seung, 2001](https://papers.nips.cc/paper/1861-algorithms-for-non-negative-matrix-factorization.pdf)|&#10003;|&#10003;|

## Quick start

Follow the installation instructions given in the INSTALL file. First time installation and compilation may take while.

To generate some synthetic data, run:

    cd build/tools
    ./generateSyntheticData

The script `generateSyntheticData` will generate the following files:
- `/tmp/train.mmc`: a matrix with some training data
- `/tmp/test.mmc`: a matrix with some test data
- `/tmp/W.mma` and `/tmp/H.mma`: initial factor matrices

To run DSGD++ on a single-machine with 4 threads, type:

    ./mfdsgdpp --tasks-per-rank=4 --input-file=/tmp/train.mmc --input-test-file=/tmp/test.mmc --input-row-file=/tmp/W.mma --input-col-file=/tmp/H.mma --rank=10 --update="Nzsl_L2(1)" --regularize="None" --loss="Nzsl_L2(1)" --decay="BoldDriver(0.001)" --epochs=50 --trace=/tmp/trace.R

This gives you output such as:

    177 mpi2 | Initialized mpi2 on 1 rank(s)
	219 mpi2 | Starting task managers (parallel mode)...
	219 mpi2 | Started task manager at rank 0
	320 main | Input
	320 main |     Input file: /tmp/train.mmc
	320 main |     Input test file: /tmp/test.mmc
	320 main |     Input row factors: /tmp/W.mma
	320 main |     Input column factors: /tmp/H.mma
	320 main | Output
	320 main |     Output row factors: Disabled
	320 main |     Output column factors: Disabled
	320 main |     Trace: /tmp/trace.R (trace)
	320 main | Parallelization
	320 main |     MPI ranks: 1
	320 main |     Tasks per rank: 4
	320 main | DSGD++ options
	320 main |     Seed: 1499881717
	320 main |     Epochs: 50
	321 main |     Factorization rank: 10
	321 main |     Update function: Nzsl_L2(1)
	321 main |     Regularize function: None
	321 main |     Loss function: Nzsl_L2(1)
	321 main |     Decay: BoldDriver(0.001)
	321 main |     Balancing: Disabled
	321 main |     Absolute function: Disabled
	321 main |     Truncation: Disabled
	321 main |     SGD step sequence: WOR
	321 main |     DSGD stratum sequence: COWOR
	367 mf   | File '/tmp/train.mmc' is not blocked; it will be blocked automatically
	368 mf   | Constructing blocks (0,0) (0,1) (0,2) (0,3) (0,4) (0,5) (0,6) (0,7) (1,0) (1,1) (1,2) (1,3) (1,4) (1,5) (1,6) (1,7) (2,0) (2,1) (2,2) (2,3) (2,4) (2,5) (2,6) (2,7) (3,0) (3,1) (3,2) (3,3) (3,4) (3,5) (3,6) (3,7)  of '/tmp/train.mmc'
    1100 mf   | File '/tmp/test.mmc' is not blocked; it will be blocked automatically
    1101 mf   | Constructing blocks (0,0) (0,1) (0,2) (0,3) (0,4) (0,5) (0,6) (0,7) (1,0) (1,1) (1,2) (1,3) (1,4) (1,5) (1,6) (1,7) (2,0) (2,1) (2,2) (2,3) (2,4) (2,5) (2,6) (2,7) (3,0) (3,1) (3,2) (3,3) (3,4) (3,5) (3,6) (3,7)  of '/tmp/test.mmc'
    1172 mf   | Test matrix: 100000 x 100000, 100000 nonzeros, 4 x 8 blocks
    1172 mf   | Data matrix: 100000 x 100000, 1000000 nonzeros, 4 x 8 blocks
    1172 mf   | File '/tmp/W.mma' is not blocked; it will be blocked automatically
    1173 mf   | Constructing blocks (0,0) (1,0) (2,0) (3,0)  of '/tmp/W.mma'
    1532 mf   | Row factor matrix: 100000 x 10, 4 x 1 blocks
    1532 mf   | File '/tmp/H.mma' is not blocked; it will be blocked automatically
    1532 mf   | Constructing blocks (0,0) (0,1) (0,2) (0,3) (0,4) (0,5) (0,6) (0,7)  of '/tmp/H.mma'
    1960 mf   | Column factor matrix: 10 x 100000, 1 x 8 blocks
    1972 main | Total time for loading matrices: 1.59269s
    2051 main | Using NzslLoss for test data
    2051 mf   | Starting DSGD++ (polling delay: 500 microseconds)
    2051 mf   | Using COWOR order for selecting strata
    2051 mf   | Using WOR order for selecting training points
    2092 mf   | Loss: 1.00091e+09 (0.040013s)
    2101 mf   | Test loss: 9.98073e+07 (0.009003s)
    2101 mf   | Step size: 0.001 (0s)
    2101 mf   | Starting epoch 1
    2257 mf   | Finished epoch 1 (0.1566s)
    2301 mf   | Loss: 9.94144e+08 (0.043457s)
    2307 mf   | Test loss: 9.98113e+07 (0.005742s)
    2307 mf   | Step size: 0.00105 (0s)
    2307 mf   | Starting epoch 2
    2468 mf   | Finished epoch 2 (0.160963s)
    2508 mf   | Loss: 9.84892e+08 (0.040107s)
    2515 mf   | Test loss: 9.98201e+07 (0.006841s)
    2515 mf   | Step size: 0.0011025 (0s)

    ...   

To get some idea about the performance of the algorithm, start an `R` console and type:

    source("mfplot.R")
    source("/tmp/trace.R")                               # created by running DSGD++ above
    mfplot(trace, x.is="time$elapsed", log="y")          # plots the training loss over time

To run DSGD++ on two machines (here both localhost) with 4 threads each, type:

    mpirun --hosts localhost,localhost ./mfdsgdpp --tasks-per-rank=4 --input-file=/tmp/train.mmc --input-test-file=/tmp/test.mmc --input-row-file=/tmp/W.mma --input-col-file=/tmp/H.mma --rank=10 --update="Nzsl_L2(1)" --regularize="None" --loss="Nzsl_L2(1)" --decay="BoldDriver(0.001)" --epochs=50 --trace=/tmp/trace.R

## Preparing data 

To use DSGDpp, convert your data to a [market matrix format](http://math.nist.gov/MatrixMarket/formats.html). We also provide tools to generate and convert matrices (e.g., `mfgenerate`, `mfconvert`, `mfsample`, `mfdblock`).

To run a factorization in a shared-nothing environment, the data must lie in a shared filesystem mounted at the same location on all machines.

Note that many algorithms require the data to be blocked in some form. The required blocking will be created on the fly, but we did not optimize the performance of this step. For faster data loading, matrices should be blocked up front via the `mfdblock` tool.

## Tools

We provide a number of tools to run factorizations and prepare data. All tools are located under `build/tools`. Run a tool executable without arguments for help on its parameters. E.g., runnung `./mfdsgdpp` produces

    mfsgdpp [options]
    
    Options:
      --help                produce help message
      --input-file arg      filename of data matrix
      --input-test-file arg filename of test matrix
      --input-row-file arg  filename of initial row factors
      --input-col-file arg  filename of initial column factors
      --output-row-file arg filename of final row factors                                               
      --output-col-file arg filename of final column factors                                            
      --trace arg           filename of trace [trace.R]                                                 
      --trace-var arg       variable name for trace [traceVar]                                          
      --epochs arg          number of epochs to run [10]                                                
      --tasks-per-rank arg  number of concurrent tasks per rank [1]                                     
      --sgd-order arg       order of SGD steps [WOR] (e.g., "SEQ", "WR", "WOR")                         
      --stratum-order arg   order of strata [COWOR] (e.g., "SEQ", "RSEQ", "WR",                         
                            "WOR", "COWOR")                                                             
      --seed arg            seed for random number generator (system time if not                        
                            set)                                                                        
      --rank arg            rank of factorization                                                       
      --update arg          SGD update function (e.g., "Sl", "Nzsl", "GklData")                         
      --regularize arg      SGD regularization function (e.g., "None", "L2(0.05)",                      
                            "Nzl2(0.05)")
      --loss arg            loss function (e.g., "Nzsl", "Nzsl_L2(0.5)"))
      --abs                 if present, absolute values are taken after every SGD 
                            step
      --truncate arg        if present, truncatation is enabled (e.g., --truncate 
                            "(-1000, 1000)"
      --decay arg           decay function (constant, bold driver, or auto)
      --balance arg         Type of balancing (None, L2, Nzl2)

The source code contains additional information about accepted parameter values. 

### Factorizating matrices

To run a method in a shared-nothing environment, simply prefix the command with `mpirun --hosts <list-of-hostnames>`.

| Tool | Method(s) |
| ---- | ------ |
| mfasgd | Asynchronous SGD |
| mfdsgdpp | DSGD++ |
| psgdL2Lock | Parallel SGD with locking, L2 loss |
| psgdL2NoLock | Parallel SGD without locking (Hogwild), L2 loss |
| psgdNZL2Lock | Parallel SGD with locking, NZL2 loss |
| psgdNZL2NoLock | Parallel SGD without locking (Hogwild), NZL2 loss |
| mfdap | Alternating least squares, non-negative matrix factorization (via multiplicative updates) |
| mfdsgd | DSGD |
| mfsgd | Sequential SGD |
| stratified-psgd | CSGD (cache-conscious parallel SGD) |

### Utilities

| Tool | Description |
| ---- | ----------- |
| averageOutMatrix | Center a matrix (substract mean from every observed entry) |
| mfconvert | Convert between different file formats |
| mfcreateInitialFactors | Create initial factor matrices |
| mfcreateRandomMatrixFile | Create a blocked random matrix |
| mfgenerate | Geneate random matrices |
| mfproject | Drops rows/columns with fewer entries than a given threshold |
| mfdblock | Blocks an input matrix |
| mfsample | Creates a sample of an input matrix (e.g., used for automatic step size selection) |
| mfprepare | Combines mfsample, mfproject, and mfcreateInitialFactors |

### References

R. Gemulla, E. Nijkamp, P. J. Haas, and Y. Sismanis
*[Large-Scale Matrix Factorization with Distributed Stochastic Gradient Descent](https://dl.acm.org/citation.cfm?id=2020426)*.
KDD, 2011

D. D. Lee and H. S. Seung
*[Algorithms for Non-negative Matrix Factorization](https://papers.nips.cc/paper/1861-algorithms-for-non-negative-matrix-factorization.pdf)*.
NIPS, 2001

F. Makari, C. Teflioudi, R. Gemulla, P. J. Haas, and Y. Sismanis
*[Shared-Memory and Shared-Nothing Stochastic Gradient Descent Algorithms for Matrix Completion](https://link.springer.com/article/10.1007/s10115-013-0718-7)*.
KAIS, 2013

F. Niu, B. Recht, C. Re, and S. J. Wright
*[Hogwild!: A lock-free approach to parallelizing stochastic gradient descent](https://dl.acm.org/citation.cfm?id=2986537)*.
NIPS, 2011

C. Teflioudi, F. Makari, and R. Gemulla
*[Distributed Matrix Completion](http://ieeexplore.ieee.org/document/6413862)*.
ICDM, 2012.
    
Y. Zhou, D. Wilkinson, R. Schreiber and R. Pan
*[Large-scale parallel collaborative filtering for the Netflix Prize](https://link.springer.com/chapter/10.1007/978-3-540-68880-8_32)*.
AAIM, 2008
