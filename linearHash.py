import sys
import copy
from collections import deque


class Block():

	def __init__(self,value = None,is_overflow = False):

	    self.value = value
	    self.overflow = is_overflow

class Bucket():

	def __init__(self,mod_val,bucket_size):

		self.block_list = [] 
		self.mod_val = 	mod_val
		self.bucket_size = bucket_size

class Hash_Table():

	def __init__(self,bucket_size,load_factor):
		
		self.split_index_ptr = 0
		self.iteration = 0
		self.m = 2
		self.size = 0
		self.overflow_element = 0
		self.overflow = 0
		self.num_elements = 0
		self.load_factor = load_factor
		self.bucket_list = []
		self.bucket_size = bucket_size
		self.global_mod_val = (self.m * (2**(self.iteration)))
		# self.

	def is_overflow_detected(self,hash_table_index):
		if len(self.bucket_list[hash_table_index].block_list) >= self.bucket_size:
			return True
		else :
			return False

	def split_condition(self,hash_table_index):

		# Split when overflow is detected
		# if len(self.bucket_list[hash_table_index].block_list) >= self.bucket_size:
		# 	return True
		# else :
		# 	return False	
		
		# Split on a load factor
		# num_elements = self.num_elements
		# size = self.size * self.bucket_size

		if self.size==0:
			return False;

		if  self.load_factor < float(self.num_elements)/float(self.size * self.bucket_size):
			return True
		else:
			return False

	def insert(self,key):
		
		# key = ((key % self.global_mod_val) + self.global_mod_val) % self.global_mod_val

		if self.search(key,from_insert=True):

			return False

		hash_table_index = self.generate_insert_index(key) 

		self.num_elements += 1
						
		while True:

			try:

				if self.split_condition(hash_table_index):
					
					# print("overflow detected")
					if self.is_overflow_detected(is_overflow_detected):
						self.bucket_list[hash_table_index].block_list.append(Block(value = key,is_overflow = True))
					else:
						self.bucket_list[hash_table_index].block_list.append(Block(value = key))
					# if len(self.bucket_list[hash_table_index].block_list) > self.bucket_size:
						#self.overflow += 1#len(self.bucket_list[hash_table_index].block_list) - self.bucket_size;					

					self.bucket_list.append(self.create_bucket())

					self.size += 1

					temp_block_list = copy.deepcopy(self.bucket_list[self.split_index_ptr].block_list)

					if len(temp_block_list) > self.bucket_size:

						self.overflow -= len(temp_block_list) - self.bucket_size					

					self.overflow += len(self.bucket_list[hash_table_index].block_list) - self.bucket_size;					

					self.bucket_list[self.split_index_ptr].block_list = []

					for block in temp_block_list:

						index = self.generate_insert_index(block.value,split = True)

						if len(self.bucket_list[index].block_list) < self.bucket_size:
							
							block.overflow = False
						else:
							#self.overflow += 1
							block.overflow = True

						self.bucket_list[index].block_list.append(block)	

					self.bucket_list[self.split_index_ptr].mod_val = self.m * 2**(self.iteration+1)

					self.split_index_ptr += 1					

					if self.split_index_ptr ==  self.global_mod_val:

						self.split_index_ptr = 0
						self.iteration += 1
						self.global_mod_val = self.m * 2**self.iteration										
				
				else:
					self.bucket_list[hash_table_index].block_list.append(Block(value = key))

				break

			except IndexError:

				self.bucket_list.append(self.create_bucket())

		return True

	def create_bucket(self):

		bucket = Bucket(mod_val = self.global_mod_val, bucket_size = self.bucket_size)

		return bucket

	def generate_insert_index(self,key,split = False):


		if split == False:

			hash_table_index = ((key % self.global_mod_val) + self.global_mod_val) % self.global_mod_val
			
			# hash_table_index = key % self.global_mod_val

			if hash_table_index < self.split_index_ptr:

				hash_table_index = key % (self.m * (2**(self.iteration+1)))

			return hash_table_index

		elif split == True:
			
			return key % (self.m * (2**(self.iteration+1)))			

	def search(self,key,from_insert=False):
		
		hash_table_index = self.generate_insert_index(key)
		
		try:
			for block in self.bucket_list[hash_table_index].block_list:

				if key == block.value:
					# if from_insert == False:
						# print("found")
					return True
		except:
			# if from_insert == False:
				# print("not found")
			return False
		# if from_insert == False:
			# print("not found")
		return False	

	def print_hash_table(self):
		print("Start")
		for bucket in self.bucket_list:

			for block in bucket.block_list:

				print(block.value,end =", ")
			print()
		print("End")


def clear_output():
	
	global ht, input_size, output_size, input_buf, output_buf, f_input, f_output

	for token in output_buf:

		f_output.write(str(token)+"\n")


	output_buf = []

    
def clear_input():

	global ht, input_size, output_size, input_buf, output_buf, f_input, f_output
	i = 0
	j = 0
	for token in input_buf:
		i += 1
		if ht.insert(token) == True:
			j += 1

			# print(len(output_buf), output_size)

			if len(output_buf) >= output_size:
				# print("length of output buf: ",len(output_buf))
				# print("before: ",output_buf)
				clear_output()
				output_buf = []
				# print("after: ",output_buf)

				# print("length of output buf: ",len(output_buf))

			output_buf.append(token)

	# print(i)
	# print(j)
	input_buf = []



if __name__ == '__main__':

	
	if len(sys.argv) != 5:

		print("Invalid input format")
		print("[executable] [input file] [number of buffers eg. 32] [buffer size eg. 64 B]")
		sys.exit()

	filename = str(sys.argv[1]);
	num_buffers = int(sys.argv[2])
	buffer_size = int(sys.argv[3])
	unit = str(sys.argv[4]);

    
	if unit == "B":
		buffer_size = buffer_size

	elif unit == "KB":
		buffer_s = buffer_s * 1000;

	elif unit == "MB":
		buffer_s = buffer_s * 1000000;

	else:
		print("Invalid buffer size provided Expected B, KB, MB only")
		sys.exit()        


	if num_buffers < 2:
		cout<<"Number of buffer must be atleast greater than 2"
		sys.exit()
   



	global ht, input_size, output_size, input_buf, output_buf, f_input, f_output

	ht = Hash_Table(1000,0.75)

	input_size = (num_buffers - 1) * (buffer_size / 4);
	output_size = buffer_sizeoutput_size / 4;

	# print(input_size)
	# print(output_size)

	input_buf = [];
	output_buf = [];

	f_output = open('output_lh.txt','w')
	f_output.close()


	f_input = open(filename)
	f_output = open('output_lh.txt','a')

	line = f_input.readline()
	index = 1
	while line:
		# print(line)
		# print("input size: ",input_size)
		# print("input size: ",output_size)
		# print("length of input buf: ",len(input_buf))
		# print("length of output buf: ",len(output_buf))
		# index += 1

		if len(input_buf) >= input_size:
			# print("here")
			# print("before: ",input_buf)
			# print("before: ",output_buf)
			clear_input()
			# print("after: ",input_buf)
			# print("after: ",output_buf)
			# print("length of input buf: ",len(input_buf))
            
		input_buf.append(int(line))

		line = f_input.readline()

	f_input.close()

	clear_input()
	clear_output()

	f_output.close()

	# ht.insert(1)
	# ht.print_hash_table()
	# ht.insert(2)
	# ht.print_hash_table()
	# ht.insert(3)
	# ht.print_hash_table()
	# ht.insert(5)
	# ht.print_hash_table()
	# ht.insert(8)
	# ht.print_hash_table()
	# ht.insert(13)
	# ht.print_hash_table()
	# ht.insert(21)
	# ht.print_hash_table()

	# ht.search(0)
	# ht.search(1)
	# ht.search(2)
	# ht.search(3)
