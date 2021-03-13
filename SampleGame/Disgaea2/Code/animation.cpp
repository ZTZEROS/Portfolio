#include "stdafx.h"
#include "animation.h"


animation::animation()
	: _frameNum(0),
	_frameWidth(0),
	_frameHeight(0),
	_loop(FALSE),
	_frameUpdateSec(0),
	_elapsedSec(0),
	_nowPlayIdx(0),
	_play(FALSE)
{
}


animation::~animation()
{
}

HRESULT animation::init(int totalW, int totalH, int frameW, int frameH)
{
	//���� ������ ����
	_frameWidth = frameW;
	int _frameNumWidth = totalW / _frameWidth;

	//���� ������ ����
	_frameHeight = frameH;
	int _frameNumHeight = totalH / _frameHeight;

	//�� ������ �� ���
	_frameNum = _frameNumWidth * _frameNumHeight;

	//������ ��ġ ����Ʈ ����
	for (int i = 0; i < _frameNumHeight; i++)
	{
		for (int j = 0; j < _frameNumWidth; j++)
		{
			POINT framePos;
			framePos.x = j * _frameWidth;
			framePos.y = i * _frameHeight;

			_frameList.push_back(framePos);
		}
	}

	//�ϴ� �⺻ ���������� ����
	setDefPlayFrame();

	return S_OK;
}

void animation::release(void)
{

}

//Default �ִϸ��̼�
void animation::setDefPlayFrame(BOOL reverse, BOOL loop)
{
	_obj = NULL;
	_callbackFunction = NULL;
	_callbackFunctionParameter = NULL;


	//���� ����
	_loop = loop;

	//�÷��� ����Ʈ Ŭ����
	_playList.clear();

	//������ ->
	//������(������ <-

	if (reverse)
	{
		if (_loop)
		{
			for (int i = 0; i < _frameNum; i++)
			{
				_playList.push_back(i);
			}

			for (int i = _frameNum - 1; i > 0; i--)
			{
				_playList.push_back(i);
			}
		}
		else
		{

			for (int i = _frameNum - 1; i > 0; i--)
			{
				_playList.push_back(i);
			}
		}
	}
	else
	{
		for (int i = 0; i < _frameNum; i++)
		{
			_playList.push_back(i);
		}
	}

}

void animation::setDefPlayFrame(BOOL reverse, BOOL loop, CALLBACKFUNCTION cbFunction)
{
	_obj = NULL;
	_callbackFunction = cbFunction;
	_callbackFunctionParameter = NULL;


	//���� ����
	_loop = loop;

	//�÷��� ����Ʈ Ŭ����
	_playList.clear();

	//������ ->
	//������(������ <-

	if (reverse)
	{
		if (_loop)
		{
			for (int i = 0; i < _frameNum; i++)
			{
				_playList.push_back(i);
			}

			for (int i = _frameNum - 1; i > 0; i--)
			{
				_playList.push_back(i);
			}
		}
		else
		{
			for (int i = 0; i < _frameNum; i++)
			{
				_playList.push_back(i);
			}

			for (int i = _frameNum - 1; i > 0; i--)
			{
				_playList.push_back(i);
			}
		}
	}
	else
	{
		for (int i = 0; i < _frameNum; i++)
		{
			_playList.push_back(i);
		}
	}
}

void animation::setDefPlayFrame(BOOL reverse, BOOL loop, CALLBACKFUNCTION_PARAMETER cbFunction, void* obj)
{
	_obj = obj;
	_callbackFunction = NULL;
	_callbackFunctionParameter = cbFunction;


	//���� ����
	_loop = loop;

	//�÷��� ����Ʈ Ŭ����
	_playList.clear();

	//������ ->
	//������(������ <-

	if (reverse)
	{
		if (_loop)
		{
			for (int i = 0; i < _frameNum; i++)
			{
				_playList.push_back(i);
			}

			for (int i = _frameNum - 1; i > 0; i--)
			{
				_playList.push_back(i);
			}
		}
		else
		{
			for (int i = 0; i < _frameNum; i++)
			{
				_playList.push_back(i);
			}

			for (int i = _frameNum - 1; i > 0; i--)
			{
				_playList.push_back(i);
			}
		}
	}
	else
	{
		for (int i = 0; i < _frameNum; i++)
		{
			_playList.push_back(i);
		}
	}
}


//�迭�� ������ �ִϸ��̼�
void animation::setPlayFrame(int* playArr, int arrLen, BOOL loop)
{
	_obj = NULL;
	_callbackFunction = NULL;
	_callbackFunctionParameter = NULL;

	//���� ����
	_loop = loop;

	//�÷��� ����Ʈ Ŭ����
	_playList.clear();

	for (int i = 0; i < arrLen; i++)
	{
		_playList.push_back(playArr[i]);
	}
}

void animation::setPlayFrame(int* playArr, int arrLen, BOOL loop, CALLBACKFUNCTION cbFunction)
{
	_obj = NULL;
	_callbackFunction = cbFunction;
	_callbackFunctionParameter = NULL;

	//���� ����
	_loop = loop;

	//�÷��� ����Ʈ Ŭ����
	_playList.clear();

	for (int i = 0; i < arrLen; i++)
	{
		_playList.push_back(playArr[i]);
	}
}

void animation::setPlayFrame(int* playArr, int arrLen, BOOL loop, CALLBACKFUNCTION_PARAMETER cbFunction, void* obj)
{
	_obj = obj;
	_callbackFunction = NULL;
	_callbackFunctionParameter = cbFunction;

	//���� ����
	_loop = loop;

	//�÷��� ����Ʈ Ŭ����
	_playList.clear();

	for (int i = 0; i < arrLen; i++)
	{
		_playList.push_back(playArr[i]);
	}
}


//���۰� �� ������ �ִ� �ִϸ��̼�
void animation::setPlayFrame(int start, int end, BOOL reverse, BOOL loop)
{
	_obj = NULL;
	_callbackFunction = NULL;
	_callbackFunctionParameter = NULL;

	//���� ����
	_loop = loop;

	//�÷��� ����Ʈ Ŭ����
	_playList.clear();

	//���۰� ���� ������
	if (start == end)
	{
		_playList.clear();
		stop();
		return;
	}

	//������������ ���������Ӻ��� ū ���
	if (start > end)
	{
		//��������?
		if (reverse)
		{
			//������?
			if (_loop)
			{
				for (int i = start; i >= end; i--)
				{
					_playList.push_back(i);
				}

				for (int i = end + 1; i < start; i++)
				{
					_playList.push_back(i);
				}
			}
			else
			{
				for (int i = start; i >= end; i--)
				{
					_playList.push_back(i);
				}

				for (int i = end + 1; i < start; i++)
				{
					_playList.push_back(i);
				}
			}
		}
		else
		{
			for (int i = start; i >= end; i--)
			{
				_playList.push_back(i);
			}
		}

	}
	else
	{
		//��������?
		if (reverse)
		{
			//������?
			if (_loop)
			{
				for (int i = start; i < end; i++)
				{
					_playList.push_back(i);
				}

				for (int i = end; i >= start; i--)
				{
					_playList.push_back(i);
				}


			}
			else
			{
				for (int i = start; i < end; i++)
				{
					_playList.push_back(i);
				}

				for (int i = end; i >= start; i--)
				{
					_playList.push_back(i);
				}
			}
		}
		else
		{
			for (int i = start; i < end; i++)
			{
				_playList.push_back(i);
			}
		}
	}
}

void animation::setPlayFrame(int start, int end, BOOL reverse, BOOL loop, CALLBACKFUNCTION cbFunction)
{
	_obj = NULL;
	_callbackFunction = cbFunction;
	_callbackFunctionParameter = NULL;

	//���� ����
	_loop = loop;

	//�÷��� ����Ʈ Ŭ����
	_playList.clear();

	//���۰� ���� ������
	if (start == end)
	{
		_playList.clear();
		stop();
		return;
	}

	//������������ ���������Ӻ��� ū ���
	if (start > end)
	{
		//��������?
		if (reverse)
		{
			//������?
			if (_loop)
			{
				for (int i = start; i >= end; i--)
				{
					_playList.push_back(i);
				}

				for (int i = end + 1; i < start; i++)
				{
					_playList.push_back(i);
				}
			}
			else
			{
				for (int i = start; i >= end; i--)
				{
					_playList.push_back(i);
				}

				for (int i = end + 1; i < start; i++)
				{
					_playList.push_back(i);
				}
			}
		}
		else
		{
			for (int i = start; i >= end; i--)
			{
				_playList.push_back(i);
			}
		}

	}
	else
	{
		//��������?
		if (reverse)
		{
			//������?
			if (_loop)
			{
				for (int i = start; i < end; i++)
				{
					_playList.push_back(i);
				}

				for (int i = end; i >= start; i--)
				{
					_playList.push_back(i);
				}


			}
			else
			{
				for (int i = start; i < end; i++)
				{
					_playList.push_back(i);
				}

				for (int i = end; i >= start; i--)
				{
					_playList.push_back(i);
				}
			}
		}
		else
		{
			for (int i = start; i < end; i++)
			{
				_playList.push_back(i);
			}
		}
	}
}

void animation::setPlayFrame(int start, int end, BOOL reverse, BOOL loop, CALLBACKFUNCTION_PARAMETER cbFunction, void* obj)
{
	_obj = obj;
	_callbackFunction = NULL;
	_callbackFunctionParameter = cbFunction;

	//���� ����
	_loop = loop;

	//�÷��� ����Ʈ Ŭ����
	_playList.clear();

	//���۰� ���� ������
	if (start == end)
	{
		_playList.clear();
		stop();
		return;
	}

	//������������ ���������Ӻ��� ū ���
	if (start > end)
	{
		//��������?
		if (reverse)
		{
			//������?
			if (_loop)
			{
				for (int i = start; i >= end; i--)
				{
					_playList.push_back(i);
				}

				for (int i = end + 1; i < start; i++)
				{
					_playList.push_back(i);
				}
			}
			else
			{
				for (int i = start; i >= end; i--)
				{
					_playList.push_back(i);
				}

				for (int i = end + 1; i < start; i++)
				{
					_playList.push_back(i);
				}
			}
		}
		else
		{
			for (int i = start; i >= end; i--)
			{
				_playList.push_back(i);
			}
		}

	}
	else
	{
		//��������?
		if (reverse)
		{
			//������?
			if (_loop)
			{
				for (int i = start; i < end; i++)
				{
					_playList.push_back(i);
				}

				for (int i = end; i >= start; i--)
				{
					_playList.push_back(i);
				}


			}
			else
			{
				for (int i = start; i < end; i++)
				{
					_playList.push_back(i);
				}

				for (int i = end; i >= start; i--)
				{
					_playList.push_back(i);
				}
			}
		}
		else
		{
			for (int i = start; i < end; i++)
			{
				_playList.push_back(i);
			}
		}
	}
}


void animation::setFPS(int framePerSec)
{
	_frameUpdateSec = 1.0f / framePerSec;
}

void animation::frameUpdate(float elapsedTime)
{
	if (_play)
	{
		_elapsedSec += elapsedTime;

		//������ ������Ʈ �ð��� �Ǿ�����
		if (_elapsedSec >= _frameUpdateSec)
		{
			_elapsedSec -= _frameUpdateSec;
			_nowPlayIdx++;

			if (_nowPlayIdx == _playList.size())
			{
				if (_loop)
				{
					_nowPlayIdx = 0;
				}
				else
				{
					if (_obj == NULL)
					{
						if (_callbackFunction != NULL) _callbackFunction();
					}
					else
					{
						_callbackFunctionParameter(_obj);
					}
					_nowPlayIdx--;
					_play = FALSE;
				}
			}
		}
	}
}

void animation::start(void)
{
	_play = TRUE;
	_nowPlayIdx = 0;
}

void animation::stop(void)
{
	_play = FALSE;
	_nowPlayIdx = 0;
}

void animation::pause(void)
{
	_play = FALSE;
}

void animation::resume(void)
{
	_play = TRUE;
}
