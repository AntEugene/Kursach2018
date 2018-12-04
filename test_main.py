from main import pyTest
import pytest


class TestMain:

    def test_succes(self):
        assert(pyTest('Test Succesful!'))