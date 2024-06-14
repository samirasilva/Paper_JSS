#!/usr/bin/env Rscript


#http://www.sthda.com/english/wiki/kruskal-wallis-test-in-r


# If .txt tab file, use this
my_data <- read.delim("kruskal_data.txt")

#install if it is not installed
#install.packages("dplyr")

library(dplyr)
group_by(my_data, group) %>%
  summarise(
    count = n(),
    mean = mean(as.numeric(weight), na.rm = TRUE),
    sd = sd(as.numeric(weight), na.rm = TRUE),
    median = median(as.numeric(weight), na.rm = TRUE),
    IQR = IQR(as.numeric(weight), na.rm = TRUE)
  )

kruskal.test(weight ~ group, data = my_data)


