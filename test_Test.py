from Test import Test
import pytest

class TestIt:

	def test_type_error(self):
		with pytest.raises(TypeError):
			Test("LOL")

	def test_one(self):
		assert(Test(1) == 1)

	def test_zero(self):
		assert(Test(0) == 0)
