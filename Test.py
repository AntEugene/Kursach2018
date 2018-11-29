def Test(input):
	if type(input) is not int:
		raise TypeError("Type of num argument should be int")
	if input == 1:
		return input
	elif input == 0:
		return input
	else:
		output = -1
		return output

if __name__ == "__main__":
	print(Test("IO"))
