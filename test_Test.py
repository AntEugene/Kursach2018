from Test import Test
import pytest

class TestIt:

	def test_type_error(self):
		with pytest.raises(TypeError):
			SetTsk_name("Tested")
