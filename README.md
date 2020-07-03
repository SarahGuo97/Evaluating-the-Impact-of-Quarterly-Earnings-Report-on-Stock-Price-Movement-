# Evaluating-the-Impact-of-Quarterly-Earnings-Report-on-Stock-Price-Movement-

This project is built in C++.
•	Retrieved historical price data using Libcurl from Yahoo Finance; stored EPS information in an STL map with tickers as keys
•	Sorted all S&P 500 stocks by Surprise Ratio and split into three groups from highest to lowest: Beat, Meet and Miss
•	Implemented Bootstrapping method to simulate and compute Average Abnormal Return (AAR), Cumulative AAR and their standard deviations for each group; reduced runtime by 67% using multi-threads
•	Visualized results with Gnuplot; Quarterly Earnings Report announcement has a significant impact on Beat and Miss groups; AAR increased by 3% for Beat group and decreased by 2% for Miss group
