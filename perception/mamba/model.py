import torch
import numpy as np

from mamba_ssm import Mamba2
from einops import rearrange
from torch.utils.tensorboard import SummaryWriter


device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

print(f"Using device: {device}")

