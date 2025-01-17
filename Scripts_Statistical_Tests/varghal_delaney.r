#!/usr/bin/env Rscript

# Explanation of results: https://rdrr.io/cran/rcompanion/man/vda.html
# How to use the vd.a function https://search.r-project.org/CRAN/refmans/effsize/html/VD.A.html

#install.packages("effsize")
library(effsize)

group1 <- c(98.15,98.18,98.15,98.19,98.15,98.16,98.19,98.19,98.14,98.25,98.2,98.15,98.16,98.23,98.13,98.16,98.2,98.21,98.19,98.2,98.22,98.18,98.18,98.2,98.16) #Random
group2 <- c(93.59,93.1,93.34,93.29,93.36,93.02,93.23,93.59,93.79,93.03,93.24,93.22,92.16,92.18,92.45,91.44,92.22,92.71,92.47,92.16,92.24,92.54,92.59,92.86,92.12) #PASTA
group3 <- c(100.00,79.67,98.77,99.19,97.50,98.56,84.12,97.33,87.89,100.00,99.11,95.75,99.61,98.85,96.86,94.88,99.13,83.70,97.33,90.41,95.83,88.00,93.79,98.69,99.11) #Coverage
group4 <- c(95.41,95.38,95.67,95.55,94.72,95.67,96.24,95.88,95.7,95.41,95.32,95.49,95.2,95.49,95.2,95.47,95.11,95.79,95.08,95.82,95.14,95.64,95.32,95.7,95.32) #Combinatorial

# Calculate the Vargha-Delaney effect size for Random vs PASTA
print(paste("Effect size for Random vs PASTA:"))
VD.A(group1, group2)

# Calculate the Vargha-Delaney effect size for PASTA vs Random
print(paste("Effect size for PASTA vs Random:"))
VD.A(group2, group1)

# Calculate the Vargha-Delaney effect size for Random vs Coverage
print(paste("Effect size for Random vs Coverage:"))
VD.A(group1, group3)

# Calculate the Vargha-Delaney effect size for Coverage vs Random
print(paste("Effect size for Coverage vs Random:"))
VD.A(group3, group1)


# Calculate the Vargha-Delaney effect size for Random vs Combinatorial
print(paste("Effect size for Random vs Combinatorial:"))
VD.A(group1, group4)

# Calculate the Vargha-Delaney effect size for Combinatorial vs Random
print(paste("Effect size for Combinatorial vs Random:"))
VD.A(group4, group1)


# Calculate the Vargha-Delaney effect size for PASTA vs Coverage
print(paste("Effect size for PASTA vs Coverage:"))
VD.A(group2, group3)

# Calculate the Vargha-Delaney effect size for Coverage vs PASTA
print(paste("Effect size for Coverage vs PASTA:"))
VD.A(group3, group2)


# Calculate the Vargha-Delaney effect size for PASTA vs Combinatorial
print(paste("Effect size for PASTA vs Combinatorial:"))
VD.A(group2, group4)

# Calculate the Vargha-Delaney effect size for Combinatorial vs PASTA
print(paste("Effect size for Combinatorial vs PASTA:"))
VD.A(group4, group2)


# Calculate the Vargha-Delaney effect size for Coverage vs Combinatorial
print(paste("Effect size for Coverage vs Combinatorial:"))
VD.A(group3, group4)

# Calculate the Vargha-Delaney effect size for Combinatorial vs Coverage
print(paste("Effect size for Combinatorial vs Coverage:"))
VD.A(group4, group3)


