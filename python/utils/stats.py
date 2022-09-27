import typing as tp


class NotEnoughtInstancesError(Exception):
    pass


class Sample_statistics():
    def __init__(self) -> None:
        self.instances = 0
        self.running = 0
        self.running_squared = 0

    def __str__(self):
        return (
            f'Statistics for Sample statistics class:\n'
            f'{"Instances:" : <19}{self.instances : >20}\n'
            f'{"Mean:" : <19}{self.mean : >20}\n'
            f'{"Variance:" : <19}{self.var : >20}\n'
            f'{"STD:" : <19}{self.std : >20}\n'
        )

    def __len__(self):
        return self.instances

    def add(self, value: tp.Union[int, float]) -> None:
        self.instances += 1
        self.running += value
        self.running_squared += pow(value, 2)

    @property
    def mean(self) -> float:
        if self.instances < 1:
            raise NotEnoughtInstancesError(
                f'Statistics contains {self.instances} values, '
                'at least 1 needed for mean'
            )
        return self.running / self.instances

    @property
    def _mean_squared(self) -> float:
        if self.instances < 1:
            raise NotEnoughtInstancesError(
                f'Statistics contains {self.instances} values, '
                'at least 1 needed for mean'
            )
        return self.running_squared / self.instances

    @property
    def var(self) -> float:
        if self.instances < 2:
            raise NotEnoughtInstancesError(
                f'Statistics contains {self.instances} values, '
                'at least 2 needed for var'
            )
        return (
            self.instances
            / (self.instances - 1)
            * (self._mean_squared - pow(self.mean, 2))
        )

    @property
    def std(self) -> float:
        if self.instances < 2:
            raise NotEnoughtInstancesError(
                f'Statistics contains {self.instances} values, '
                'at least 2 needed for std'
            )
        return pow(self.var, 1/2)

    def clear(self) -> None:
        self.instances = 0
        self.running = 0
        self.running_squared = 0
