LFS_Sim	
		Yu Heng Chen, Jianqiu (Leo) Huang, Yexin Miao

***** The Simulator *****
To build the LFS simulator:
   	 make

To run the simulator:
         ./LFS-Sim <disk-size> <segment-size> <input-file-name>

***** The Generator *****
To build the enerator:
   	 make gen

To run the generator:
         ./GenLFS-Sim <disk-size> <write-ratio [0-100]> <output-file>

***** Results *****
We wanted to see how segment size would affect the number of seeks.

We ran our tests using a script (autorun).
   * We included one of our test results as demonstration.
   * For this test, we used a disk size of 10000, and varied the
     segment size from 10 to 1000, incrementing by 10 at each step.
   * The sequence of instructions is in trace_file_10000.txt.
   * The measurements are in trace_file_10000_results.txt.

We noticed that the trend is the total number of seeks decreases as 
   segment size increases. However, there are certain values for 
   segment size where the number of seeks increased as segment size
   increased.
   * For example, with a segment size of 120, there were 7576 seeks,
     but with a segment size of 130, there were 7586 seeks.

We noticed that the percentage of seeks (seeks / total reads/writes)
   also decreases as segment size increases. We will explain why this
   happens:
   1. Notice that the number of reads is constant, so the only value 
      that changes is the number of writes.
   2. The number of writes will increase with higher segment size,
      because the cleaning procedure has to clean out more data blocks
      when cleaning a segment. 
   3. All reads count as 1 seek, so the number of seeks is at least
      the number of reads. But since the number of reads is constant 
      (1), the seek count is only affected by the number of seeks for
      empty segments during writes. On the other hand, with larger
      segments, the file system does not have to seek as often for 
      empty segments because the file system can write more blocks to
      one segment before it is full. So as segment size increases,
      the seek count decreases. 
   4. By (1) and (2), the total number of reads/writes increases as 
      segment size increases.
   5. By (3) and (4), as segement size increases, the percentage of
      seeks decreases (since the number of seeks decreases, and the
      number of reads/writes increases).

***** Conclusions and Limitations *****
We conclude that in general, larger segment sizes will increase
   performance of a Log Structured File System. 
However, this is just a simulation; we did not account for the cost of
   writes. If we did, we expect to find a point where increasing the 
   segment size will decrease performance as the cost of more writes 
   overweighs the gain from fewer seeks. 
We can also vary the threshold of our cleaning policy, and see how
   that will affect performance. 
