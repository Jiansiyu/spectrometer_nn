



This report was automatically generated with the R package **knitr**
(version 1.31).


```r
---
title: "Spectometer Regression With All Phi Term" 
author: "**Siyu Jian / sj9va**"
date: " Wed Mar 10 | 10:55am"
output: R6018::homework
---

*******************************************
```

```
## Error: <text>:8:1: unexpected '^'
## 7: 
## 8: **
##    ^
```



# Required R packages and Directories

### {.solution}

```r
data.dir = 'https://mdporter.github.io/SYS6018/data/' # data directory
library(R6018)     # functions for SYS-6018
library(tidyverse) # functions for data manipulation   
library(mlbench)
library(glmnet)
library(glmnetUtils) 
```

# Load the focal plane training dataset 
![](https://raw.githubusercontent.com/Jiansiyu/spectrometer_nn/main/dataGenerator/resource/runList.png)

**Dataset Notics**

* **PRex_LHRS_combine_focal.csv** :: the Old Phi term removed from the dataset


```r
focal_url = "http://localhost/Data/spectro_nn/focalPlane/Equal/EqEvt731/order7_ep5/combine.csv"
dataset_focal = readr::read_csv(focal_url)
head(dataset_focal)
```

```
#> # A tibble: 6 x 319
#>   evtID runID CutID SieveRowID SieveColID    bpmX     bpmY x0th0y0ph0 x0th0y0ph1
#>   <dbl> <dbl> <dbl>      <dbl>      <dbl>   <dbl>    <dbl>      <dbl>      <dbl>
#> 1     0  2239   136          3          5 0.00379 -5.01e-4          1   -0.00162
#> 2     1  2239   130          4          4 0.00379 -5.01e-4          1   -0.00900
#> 3     2  2239   179          4         11 0.00379 -5.01e-4          1    0.0150 
#> 4     3  2239   142          2          6 0.00379 -5.01e-4          1    0.00139
#> 5     4  2239   127          1          4 0.00379 -5.01e-4          1   -0.00413
#> 6     5  2239   128          2          4 0.00379 -5.01e-4          1   -0.00379
#> # … with 310 more variables: x0th0y0ph2 <dbl>, x0th0y0ph3 <dbl>,
#> #   x0th0y0ph4 <dbl>, x0th0y0ph5 <dbl>, x0th0y1ph0 <dbl>, x0th0y1ph1 <dbl>,
#> #   x0th0y1ph2 <dbl>, x0th0y1ph3 <dbl>, x0th0y1ph4 <dbl>, x0th0y1ph5 <dbl>,
#> #   x0th0y2ph0 <dbl>, x0th0y2ph1 <dbl>, x0th0y2ph2 <dbl>, x0th0y2ph3 <dbl>,
#> #   x0th0y2ph4 <dbl>, x0th0y2ph5 <dbl>, x0th0y3ph0 <dbl>, x0th0y3ph1 <dbl>,
#> #   x0th0y3ph2 <dbl>, x0th0y3ph3 <dbl>, x0th0y3ph4 <dbl>, x0th0y4ph0 <dbl>,
#> #   x0th0y4ph1 <dbl>, x0th0y4ph2 <dbl>, x0th0y4ph3 <dbl>, x0th0y5ph0 <dbl>,
#> #   x0th0y5ph1 <dbl>, x0th0y5ph2 <dbl>, x0th1y0ph0 <dbl>, x0th1y0ph1 <dbl>,
#> #   x0th1y0ph2 <dbl>, x0th1y0ph3 <dbl>, x0th1y0ph4 <dbl>, x0th1y0ph5 <dbl>,
#> #   x0th1y1ph0 <dbl>, x0th1y1ph1 <dbl>, x0th1y1ph2 <dbl>, x0th1y1ph3 <dbl>,
#> #   x0th1y1ph4 <dbl>, x0th1y1ph5 <dbl>, x0th1y2ph0 <dbl>, x0th1y2ph1 <dbl>,
#> #   x0th1y2ph2 <dbl>, x0th1y2ph3 <dbl>, x0th1y2ph4 <dbl>, x0th1y3ph0 <dbl>,
#> #   x0th1y3ph1 <dbl>, x0th1y3ph2 <dbl>, x0th1y3ph3 <dbl>, x0th1y4ph0 <dbl>,
#> #   x0th1y4ph1 <dbl>, x0th1y4ph2 <dbl>, x0th1y5ph0 <dbl>, x0th1y5ph1 <dbl>,
#> #   x0th2y0ph0 <dbl>, x0th2y0ph1 <dbl>, x0th2y0ph2 <dbl>, x0th2y0ph3 <dbl>,
#> #   x0th2y0ph4 <dbl>, x0th2y0ph5 <dbl>, x0th2y1ph0 <dbl>, x0th2y1ph1 <dbl>,
#> #   x0th2y1ph2 <dbl>, x0th2y1ph3 <dbl>, x0th2y1ph4 <dbl>, x0th2y2ph0 <dbl>,
#> #   x0th2y2ph1 <dbl>, x0th2y2ph2 <dbl>, x0th2y2ph3 <dbl>, x0th2y3ph0 <dbl>,
#> #   x0th2y3ph1 <dbl>, x0th2y3ph2 <dbl>, x0th2y4ph0 <dbl>, x0th2y4ph1 <dbl>,
#> #   x0th2y5ph0 <dbl>, x0th3y0ph0 <dbl>, x0th3y0ph1 <dbl>, x0th3y0ph2 <dbl>,
#> #   x0th3y0ph3 <dbl>, x0th3y0ph4 <dbl>, x0th3y1ph0 <dbl>, x0th3y1ph1 <dbl>,
#> #   x0th3y1ph2 <dbl>, x0th3y1ph3 <dbl>, x0th3y2ph0 <dbl>, x0th3y2ph1 <dbl>,
#> #   x0th3y2ph2 <dbl>, x0th3y3ph0 <dbl>, x0th3y3ph1 <dbl>, x0th3y4ph0 <dbl>,
#> #   x0th4y0ph0 <dbl>, x0th4y0ph1 <dbl>, x0th4y0ph2 <dbl>, x0th4y0ph3 <dbl>,
#> #   x0th4y1ph0 <dbl>, x0th4y1ph1 <dbl>, x0th4y1ph2 <dbl>, x0th4y2ph0 <dbl>,
#> #   x0th4y2ph1 <dbl>, x0th4y3ph0 <dbl>, …
```

```r
set.seed(2000)
```


# Training the model

## pre-process the dataset, select the runs , sieves that will be used for the optimization


```r
dataSet = dataset_focal %>% 
  filter(.$SieveColID <=12 & .$SieveColID >=3)
#filter(.$runID == 2239 | .$runID == 2240 |  .$runID == 2241) %>%#

# add another weight term, how to adjust the weight term make sure it close to the theoretical value

dataSet  = dataSet %>% mutate(wght = case_when(
  runID == 2239 | runID == 2241  ~ 2*(13-SieveColID),
  TRUE ~ (13-SieveColID)
))

dataSet%>% select(runID,SieveColID,SieveRowID,wght) %>% head()
```

```
#> # A tibble: 6 x 4
#>   runID SieveColID SieveRowID  wght
#>   <dbl>      <dbl>      <dbl> <dbl>
#> 1  2239          5          3    16
#> 2  2239          4          4    18
#> 3  2239         11          4     4
#> 4  2239          6          2    14
#> 5  2239          4          1    18
#> 6  2239          4          2    18
```

```r
runList = unique(dataSet$runID)
```

### pre-pare the training and the test dataset

```r
trainSize = round(0.5*nrow(dataSet))

train = sample(nrow(dataSet), size = trainSize)
test  = - train 

trainSet = dataSet[train,]
train_X = trainSet %>% select(-evtID, -runID, -CutID,-SieveRowID,-SieveColID,-bpmX,-bpmY,-targCalTh,-targCalPh,-wght)
train_y_theta = trainSet %>% select(targCalTh)
train_y_phi = trainSet %>% select(targCalPh)
cv.weight = trainSet %>% select(wght)
```

### training the model

```r
n.fold = 10

fit.theta = cv.glmnet(as.matrix(train_X), as.matrix(train_y_theta),nfolds = n.fold)
fit.phi   = cv.glmnet(as.matrix(train_X), as.matrix(train_y_phi), weights = as.matrix(cv.weight), nfolds = n.fold)
```


```r
plot(fit.phi)
```

<img src="figure/RregVaryWeight02-Rmdunnamed-chunk-5-1.png" title="plot of chunk unnamed-chunk-5" alt="plot of chunk unnamed-chunk-5" width="49%" style="display: block; margin: auto;" />


### test on the dataset


```r
trainSetRes = trainSet %>% select(evtID, runID, CutID,SieveRowID,SieveColID,bpmX,bpmY,targCalTh,targCalPh)
train_ResX   = trainSet %>% select(-evtID, -runID, -CutID,-SieveRowID,-SieveColID,-bpmX,-bpmY,-targCalTh,-targCalPh,-wght)
train_ResX_theta = trainSet  %>% select(targCalTh)
train_ResX_phi   = trainSet  %>% select(targCalPh)

train.y.theta.pred = predict(fit.theta,newx = as.matrix(train_ResX),s="lambda.min")
colnames(train.y.theta.pred) = c("pred.theta")
trainSetRes <- cbind(trainSetRes,pred.thet = train.y.theta.pred)

train.y.phi.pred = predict(fit.phi,newx = as.matrix(train_ResX),s="lambda.min")
colnames(train.y.phi.pred) = c("pred.phi")
trainSetRes <- cbind(trainSetRes,pred.phi = train.y.phi.pred)
trainSetRes$resid.theta <- trainSetRes$targCalTh - trainSetRes$pred.theta 
trainSetRes$resid.phi <- trainSetRes$targCalPh - trainSetRes$pred.phi 

myplot <- list()
for (index.step  in 1:length(runList) ) {
  runID.step = runList[index.step]
  val = trainSetRes %>%
    filter(.$runID == runID.step)
  mypt = ggplot(val) +geom_bin2d(aes(x=pred.phi,y=pred.theta,color = "blue"),bins = 300) + geom_point(aes(y=targCalTh,x=targCalPh,color="green")) + 
    xlim(-0.025,0.025) + ylim(-0.045,0.045) + labs(title=sprintf("run_2d_%d",runID.step))
  myplot[[index.step]] = mypt
}

myplot[1]
```

```
#> [[1]]
```

<img src="figure/RregVaryWeight02-Rmdunnamed-chunk-6-1.png" title="plot of chunk unnamed-chunk-6" alt="plot of chunk unnamed-chunk-6" width="49%" style="display: block; margin: auto;" />

```r
myplot[2]
```

```
#> [[1]]
```

<img src="figure/RregVaryWeight02-Rmdunnamed-chunk-6-2.png" title="plot of chunk unnamed-chunk-6" alt="plot of chunk unnamed-chunk-6" width="49%" style="display: block; margin: auto;" />

```r
myplot[3]
```

```
#> [[1]]
```

<img src="figure/RregVaryWeight02-Rmdunnamed-chunk-6-3.png" title="plot of chunk unnamed-chunk-6" alt="plot of chunk unnamed-chunk-6" width="49%" style="display: block; margin: auto;" />

```r
myplot[4]
```

```
#> [[1]]
```

<img src="figure/RregVaryWeight02-Rmdunnamed-chunk-6-4.png" title="plot of chunk unnamed-chunk-6" alt="plot of chunk unnamed-chunk-6" width="49%" style="display: block; margin: auto;" />

```r
myplot[5]
```

```
#> [[1]]
```

<img src="figure/RregVaryWeight02-Rmdunnamed-chunk-6-5.png" title="plot of chunk unnamed-chunk-6" alt="plot of chunk unnamed-chunk-6" width="49%" style="display: block; margin: auto;" />

```r
myplot[6]
```

```
#> [[1]]
```

<img src="figure/RregVaryWeight02-Rmdunnamed-chunk-6-6.png" title="plot of chunk unnamed-chunk-6" alt="plot of chunk unnamed-chunk-6" width="49%" style="display: block; margin: auto;" />

```r
myplot[7]
```

```
#> [[1]]
```

<img src="figure/RregVaryWeight02-Rmdunnamed-chunk-6-7.png" title="plot of chunk unnamed-chunk-6" alt="plot of chunk unnamed-chunk-6" width="49%" style="display: block; margin: auto;" />

```r
myplot[8]
```

```
#> [[1]]
#> NULL
```

```r
myplot[9]
```

```
#> [[1]]
#> NULL
```


### check the residual plot 


```r
residalRes = tibble(runID = numeric(), cutID = numeric(),mean.resid.theta = numeric(),stde.resid.theta = numeric(),mean.resid.phi = numeric(),stde.resid.phi= numeric())

for (id in runList) {
    test.res = trainSetRes %>%
      filter(.$runID == id) 
    
    cutid.list = unique(test.res$CutID)
    for (cutid in cutid.list){
      test.res.cutid = test.res %>%
        filter(.$CutID == cutid)
      mean.resid.theta = mean(test.res.cutid$resid.theta)
      stde.resid.theta = sd(test.res.cutid$resid.theta)/sqrt(length(test.res.cutid$resid.theta))
      
      mean.resid.phi = mean(test.res.cutid$resid.phi)
      stde.resid.phi = sd(test.res.cutid$resid.phi)/sqrt(length(test.res.cutid$resid.phi))
      residalRes = add_row(residalRes,tibble(runID=id,cutID = cutid, mean.resid.theta = mean.resid.theta,
                                             stde.resid.theta=stde.resid.theta,
                                             mean.resid.phi = mean.resid.phi,
                                             stde.resid.phi = stde.resid.phi))
    }
}

myplot.residual <- list()
for (index.step in 1:length(runList)){
  runID.step = runList[index.step]
  myplt = residalRes %>%
    filter(.$runID == runID.step) %>%
    ggplot(aes(x=cutID,y=mean.resid.phi)) + geom_point() + ylim(-0.003,0.003) +  
    labs(title=sprintf("residual_%d",runID.step))
  myplot.residual[[index.step]] = myplt
}
myplot.residual[1]
```

```
#> [[1]]
```

<img src="figure/RregVaryWeight02-Rmdunnamed-chunk-7-1.png" title="plot of chunk unnamed-chunk-7" alt="plot of chunk unnamed-chunk-7" width="49%" style="display: block; margin: auto;" />

```r
myplot.residual[2]
```

```
#> [[1]]
```

<img src="figure/RregVaryWeight02-Rmdunnamed-chunk-7-2.png" title="plot of chunk unnamed-chunk-7" alt="plot of chunk unnamed-chunk-7" width="49%" style="display: block; margin: auto;" />

```r
myplot.residual[3]
```

```
#> [[1]]
```

<img src="figure/RregVaryWeight02-Rmdunnamed-chunk-7-3.png" title="plot of chunk unnamed-chunk-7" alt="plot of chunk unnamed-chunk-7" width="49%" style="display: block; margin: auto;" />

```r
myplot.residual[4]
```

```
#> [[1]]
```

<img src="figure/RregVaryWeight02-Rmdunnamed-chunk-7-4.png" title="plot of chunk unnamed-chunk-7" alt="plot of chunk unnamed-chunk-7" width="49%" style="display: block; margin: auto;" />

```r
myplot.residual[5]
```

```
#> [[1]]
```

<img src="figure/RregVaryWeight02-Rmdunnamed-chunk-7-5.png" title="plot of chunk unnamed-chunk-7" alt="plot of chunk unnamed-chunk-7" width="49%" style="display: block; margin: auto;" />

```r
myplot.residual[6]
```

```
#> [[1]]
```

<img src="figure/RregVaryWeight02-Rmdunnamed-chunk-7-6.png" title="plot of chunk unnamed-chunk-7" alt="plot of chunk unnamed-chunk-7" width="49%" style="display: block; margin: auto;" />

```r
myplot.residual[7]
```

```
#> [[1]]
```

<img src="figure/RregVaryWeight02-Rmdunnamed-chunk-7-7.png" title="plot of chunk unnamed-chunk-7" alt="plot of chunk unnamed-chunk-7" width="49%" style="display: block; margin: auto;" />

### check the residual plot on Column 5

```r
residalRes = tibble(runID = numeric(), cutID = numeric(),mean.resid.theta = numeric(),stde.resid.theta = numeric(),mean.resid.phi = numeric(),stde.resid.phi= numeric())

for (id in runList) {
    test.res = trainSetRes %>%
      filter(.$runID == id) %>%
      filter(.$SieveColID == 5)
    
    cutid.list = unique(test.res$CutID)
    for (cutid in cutid.list){
      test.res.cutid = test.res %>%
        filter(.$CutID == cutid)
      mean.resid.theta = mean(test.res.cutid$resid.theta)
      stde.resid.theta = sd(test.res.cutid$resid.theta)/sqrt(length(test.res.cutid$resid.theta))
      
      mean.resid.phi = mean(test.res.cutid$resid.phi)
      stde.resid.phi = sd(test.res.cutid$resid.phi)/sqrt(length(test.res.cutid$resid.phi))
      residalRes = add_row(residalRes,tibble(runID=id,cutID = cutid, mean.resid.theta = mean.resid.theta,
                                             stde.resid.theta=stde.resid.theta,
                                             mean.resid.phi = mean.resid.phi,
                                             stde.resid.phi = stde.resid.phi))
    }
}

myplot.residual <- list()
for (index.step in 1:length(runList)){
  runID.step = runList[index.step]
  myplt = residalRes %>%
    filter(.$runID == runID.step) %>%
    ggplot(aes(x=cutID,y=mean.resid.phi)) + geom_point() + ylim(-0.003,0.003) +  
    labs(title=sprintf("residual_%d",runID.step))
  myplot.residual[[index.step]] = myplt
}
myplot.residual[1]
```

```
#> [[1]]
```

<img src="figure/RregVaryWeight02-Rmdunnamed-chunk-8-1.png" title="plot of chunk unnamed-chunk-8" alt="plot of chunk unnamed-chunk-8" width="49%" style="display: block; margin: auto;" />

```r
myplot.residual[2]
```

```
#> [[1]]
```

<img src="figure/RregVaryWeight02-Rmdunnamed-chunk-8-2.png" title="plot of chunk unnamed-chunk-8" alt="plot of chunk unnamed-chunk-8" width="49%" style="display: block; margin: auto;" />

```r
myplot.residual[3]
```

```
#> [[1]]
```

<img src="figure/RregVaryWeight02-Rmdunnamed-chunk-8-3.png" title="plot of chunk unnamed-chunk-8" alt="plot of chunk unnamed-chunk-8" width="49%" style="display: block; margin: auto;" />

```r
myplot.residual[4]
```

```
#> [[1]]
```

<img src="figure/RregVaryWeight02-Rmdunnamed-chunk-8-4.png" title="plot of chunk unnamed-chunk-8" alt="plot of chunk unnamed-chunk-8" width="49%" style="display: block; margin: auto;" />

```r
myplot.residual[5]
```

```
#> [[1]]
```

<img src="figure/RregVaryWeight02-Rmdunnamed-chunk-8-5.png" title="plot of chunk unnamed-chunk-8" alt="plot of chunk unnamed-chunk-8" width="49%" style="display: block; margin: auto;" />

```r
myplot.residual[6]
```

```
#> [[1]]
```

<img src="figure/RregVaryWeight02-Rmdunnamed-chunk-8-6.png" title="plot of chunk unnamed-chunk-8" alt="plot of chunk unnamed-chunk-8" width="49%" style="display: block; margin: auto;" />

```r
myplot.residual[7]
```

```
#> [[1]]
```

<img src="figure/RregVaryWeight02-Rmdunnamed-chunk-8-7.png" title="plot of chunk unnamed-chunk-8" alt="plot of chunk unnamed-chunk-8" width="49%" style="display: block; margin: auto;" />
```

The R session information (including the OS info, R version and all
packages used):


```r
sessionInfo()
```

```
#> R version 3.6.3 (2020-02-29)
#> Platform: x86_64-pc-linux-gnu (64-bit)
#> Running under: Ubuntu 20.04.2 LTS
#> 
#> Matrix products: default
#> BLAS:   /usr/lib/x86_64-linux-gnu/atlas/libblas.so.3.10.3
#> LAPACK: /usr/lib/x86_64-linux-gnu/atlas/liblapack.so.3.10.3
#> 
#> locale:
#>  [1] LC_CTYPE=en_US.UTF-8       LC_NUMERIC=C              
#>  [3] LC_TIME=en_US.UTF-8        LC_COLLATE=en_US.UTF-8    
#>  [5] LC_MONETARY=en_US.UTF-8    LC_MESSAGES=en_US.UTF-8   
#>  [7] LC_PAPER=en_US.UTF-8       LC_NAME=C                 
#>  [9] LC_ADDRESS=C               LC_TELEPHONE=C            
#> [11] LC_MEASUREMENT=en_US.UTF-8 LC_IDENTIFICATION=C       
#> 
#> attached base packages:
#> [1] stats     graphics  grDevices utils     datasets  methods   base     
#> 
#> other attached packages:
#>  [1] glmnetUtils_1.1.8 glmnet_4.1-1      Matrix_1.2-18     mlbench_2.1-3    
#>  [5] forcats_0.5.1     stringr_1.4.0     dplyr_1.0.4       purrr_0.3.4      
#>  [9] readr_1.4.0       tidyr_1.1.2       tibble_3.0.6      ggplot2_3.3.3    
#> [13] tidyverse_1.3.0   R6018_0.1.0      
#> 
#> loaded via a namespace (and not attached):
#>  [1] Rcpp_1.0.6        lubridate_1.7.9.2 lattice_0.20-40   ps_1.5.0         
#>  [5] digest_0.6.27     assertthat_0.2.1  foreach_1.5.1     utf8_1.1.4       
#>  [9] R6_2.5.0          cellranger_1.1.0  backports_1.2.1   reprex_1.0.0     
#> [13] evaluate_0.14     httr_1.4.2        highr_0.8         pillar_1.4.7     
#> [17] rlang_0.4.10      curl_4.3          readxl_1.3.1      rstudioapi_0.13  
#> [21] labeling_0.4.2    splines_3.6.3     munsell_0.5.0     broom_0.7.4      
#> [25] compiler_3.6.3    modelr_0.1.8      xfun_0.21         pkgconfig_2.0.3  
#> [29] shape_1.4.5       tidyselect_1.1.0  codetools_0.2-16  fansi_0.4.2      
#> [33] crayon_1.4.1      dbplyr_2.1.0      withr_2.4.1       grid_3.6.3       
#> [37] jsonlite_1.7.2    gtable_0.3.0      lifecycle_0.2.0   DBI_1.1.1        
#> [41] magrittr_2.0.1    scales_1.1.1      cli_2.3.0         stringi_1.5.3    
#> [45] farver_2.0.3      fs_1.5.0          xml2_1.3.2        ellipsis_0.3.1   
#> [49] generics_0.1.0    vctrs_0.3.6       iterators_1.0.13  tools_3.6.3      
#> [53] glue_1.4.2        hms_1.0.0         parallel_3.6.3    survival_3.1-8   
#> [57] colorspace_2.0-0  rvest_0.3.6       knitr_1.31        haven_2.3.1
```

```r
Sys.time()
```

```
#> [1] "2021-05-05 03:10:40 EDT"
```

