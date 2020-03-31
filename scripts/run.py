import os
import sys

bens = []

for root, dire, filenames in os.walk('../benchmarks/dimacs'):
    for filename in filenames:
        path = os.path.join(root, filename)
        bens.append(path)



bens.sort()

L = list(sys.argv)
i = int(L[1])
h = L[2]

ben = bens[i]

for k in ['1', '2', '3', '4', '5']:
# for k in ['1']:
    os.system('./bin/' + h + ' ' + ben + ' ' + k)

# sbatch --array=0-31%10 task.slurm