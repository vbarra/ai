﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NNSharp.DataTypes;
using NNSharp.KernelDescriptors;
using NNSharp.SequentialBased.SequentialLayers;

namespace NNSharp.SequentialBased.SequentialExecutors
{
    public class DeafultAbstractLayerFactory : IAbstractLayerFactory
    {

        public DeafultAbstractLayerFactory()
        {
            factories = new List<ILayerFactory>();

            factories.Add(new Bias2DLayerFactory());
            factories.Add(new Conv2DLayerFactory());
            factories.Add(new Dense2DLayerFactory());
            factories.Add(new Input2DLayerFactory());
            factories.Add(new MaxPool2DLayerFactory());
            factories.Add(new ReLuLayerFactory());
            factories.Add(new SoftmaxLayerFactory());
        }

        public ILayer CreateProduct(IKernelDescriptor descriptor, IData weights)
        {
            ILayer layer = null;
            foreach(var fac in factories)
            {
                layer = fac.CreateProduct(descriptor, weights);

                if (layer != null)
                    return layer;
            }

            throw new Exception("No layer was created!");
        }

        private List<ILayerFactory> factories;
    }
}