import typing as tp


class NotEnoughtInstancesError(Exception):
    pass


class Sample_statistics():
    def __init__(self) -> None:
        self.instances = 0
        self.running = 0
        self.running_squared = 0

    def add(self, value: tp.Union[int, float]) -> None:
        self.instances += 1
        self.running += value
        self.running_squared += pow(value, 2)
    
    @property
    def mean(self) -> float:
        if self.instances < 1:
            raise NotEnoughtInstancesError(
                f'Statistics contains {self.instances} values, at least 1 needed for mean'
            )
        return self.running / self.instances
    
    @property
    def _mean_squared(self) -> float:
        if self.instances < 1:
            raise NotEnoughtInstancesError(
                f'Statistics contains {self.instances} values, at least 1 needed for mean'
            )
        return self.running_squared / self.instances
    
    @property
    def var(self) -> float:
        if self.instances < 2:
            raise NotEnoughtInstancesError(
                f'Statistics contains {self.instances} values, at least 2 needed for var'
            )
        return self._mean_squared - pow(self.mean, 2)
    
    @property
    def std(self) -> float:
        if self.instances < 2:
            raise NotEnoughtInstancesError(
                f'Statistics contains {self.instances} values, at least 2 needed for std'
            )
        return pow(self.instances / (self.instances - 1) * self.var, 1/2)
    
    def clear(self) -> None:
        self.instances = 0
        self.running = 0
        self.running_squared = 0
